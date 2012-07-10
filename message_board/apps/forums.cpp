#include "forums.h"
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include <cppcms/cache_interface.h>
#include <boost/lexical_cast.hpp>
#include <cppcms/http_file.h>
#include <Magick++.h>
#include <cppcms/json.h>

namespace data {

new_topic_form::new_topic_form() 
{
        using cppcms::locale::translate;
        title.message(translate("Title"));
        title.limits(1,64);
        author.message(translate("Author"));
        author.value(translate("Anon"));
        author.limits(1,64);
        comment.message(translate("Message"));
        comment.attributes_string(" cols=55 rows=6 onkeypress='key2post(event, this.form);' ");
        comment.non_empty();
        image.message(translate("Upload"));
        image.limits(0,5*1024*1024);
        submit.value(translate("Create"));
        add(title);
        add(author);
        add(comment);
        add(image);
        add(submit);
}

} // data

namespace apps {

forums::forums(cppcms::service &srv) : master(srv)
{
        mapper().assign("{1}"); // with id
        mapper().assign("");    // default
        dispatcher().assign(".*",&forums::prepare,this,0);
}

void forums::prepare_content(data::forums &c,std::string const &page)
{
        master::prepare(c);
        
        const unsigned topics_per_page=10;
        int offset= page.empty() ? 0 : atoi(page.c_str());
        
        cppdb::result r;

        r=sql<< "SELECT id,title,date "
                "FROM threads "
                "ORDER BY date DESC "
                "LIMIT ?,?" << offset*topics_per_page << topics_per_page;
        
        c.topics.reserve(topics_per_page);      
        
        for(int i=0;r.next();i++) {
                c.topics.resize(c.topics.size()+1);
                r>>c.topics[i].id>>c.topics[i].title>>c.topics[i].date;
        }
        if(c.topics.size()==topics_per_page) {
                c.next=offset+1;
        }
        else {
                c.next = 0;
        }
        if(offset>0) {
                c.prev=offset-1;
                c.prevneg=1;
        }
        else {
                c.prev = 0;
                c.prevneg = 0;
        }
        render("forums",c);
}

void forums::prepare(std::string page)
{
        if(request().request_method()=="POST") {
        data::forums c;

                c.form.load(context());
                if(c.form.validate()) {
                        cppdb::transaction tr(sql);
                        cppdb::statement st;
                        cppdb::statement nextst;
                        std::string path="";
						std::string path_thumb="";
                    	if(c.form.image.set())
                        {
		                	path = "txt";
		                	if(c.form.image.value()->mime() == "image/png") { path="png"; path_thumb="png"; }
		                	if(c.form.image.value()->mime() == "image/jpeg") { path="jpg"; path_thumb="jpg"; }
		                	if(c.form.image.value()->mime() == "image/gif") { path="gif"; path_thumb="gif"; }
		                	if(c.form.image.value()->mime() == "image/svg+xml") path="svg";
		                	if(c.form.image.value()->mime() == "audio/mpeg") path="mp3";
		                	if(c.form.image.value()->mime() == "audio/ogg") path="ogg";
		                	if(c.form.image.value()->mime() == "audio/vnd.wave") path="wav";
		                	if(c.form.image.value()->mime() == "text/css") path="css";
		                	if(c.form.image.value()->mime() == "text/csv") path="csv";
		                	if(c.form.image.value()->mime() == "text/html") path="html";
		                	if(c.form.image.value()->mime() == "text/javascript") path="js";
		                	if(c.form.image.value()->mime() == "text/xml") path="xml";
		                	if(c.form.image.value()->mime() == "video/mpeg") path="mpg";
		                	if(c.form.image.value()->mime() == "video/mp4") path="mp4";
		                	if(c.form.image.value()->mime() == "video/ogg") path="ogg";
		                	if(c.form.image.value()->mime() == "video/webm") path="webm";
		                	if(c.form.image.value()->mime() == "video/x-matroska") path="mkv";
		                	if(c.form.image.value()->mime() == "video/x-ms-wmv") path="wmv";
		                	if(c.form.image.value()->mime() == "video/x-flv") path="flv";
		                	if(c.form.image.value()->mime() == "application/pdf") path="pdf";
		                	if(c.form.image.value()->mime() == "application/zip") path="zip";
		                	if(c.form.image.value()->mime() == "application/x-gzip") path="gz";
		                	if(c.form.image.value()->mime() == "application/x-tar") path="tar";
		                	if(c.form.image.value()->mime() == "application/x-deb") path="deb";
		                	if(c.form.image.value()->mime() == "application/x-rar") path="rar";
                        }
                        st= sql <<"INSERT INTO threads(title) VALUES(?)" 
                                << c.form.title.value() << cppdb::exec;
                        int tid=st.last_insert_id();
                        nextst= sql<<   "INSERT INTO messages(thread_id,file,reply_to,content,author) "
                                "VALUES (?,?,0,?,?)"
                                << tid << path << c.form.comment.value() << c.form.author.value() << cppdb::exec;
                        int id=nextst.last_insert_id();
                        if(c.form.image.set())
                        {   
                        	std::stringstream ss;
                        	ss<<settings().get<std::string>("mb.uploads")<<tid<<"_"<<id<<"."<<path;
                    		path="";
                        	ss>>path;
						    c.form.image.value()->save_to(path);
                        }

                        if(path_thumb!="")
                        {
                        	std::stringstream ss2;
                        	ss2<<settings().get<std::string>("mb.uploads")<<"thumb_"<<tid<<"_"<<id<<"."<<path_thumb;
                    		path_thumb="";
                        	ss2>>path_thumb;
		                	  try {
									Magick::Image thumb;
									thumb.read(path);
									thumb.resize("200x200>");
									thumb.write(path_thumb);
							        st=sql<<   "UPDATE messages "
							                "SET thumb=1 "
							                "WHERE id=? " 
							                << id << cppdb::exec;

								  }
								  catch( std::exception &error_ )
									{
									}
                        }
                        
                        // We need to invalidate all pages on new post
                        cache().rise("new_thread");
                        tr.commit();

                        response().set_redirect_header(url("/user_thread",tid));
                        c.form.clear();
                        return;
                }
                else {
                        prepare_content(c,page);
                }
        }
        else {
                std::string key = "main_page_" + page;

                if(cache().fetch_page(key))
                        return;
                // Add some shared key for all main_page_
                cache().add_trigger("new_thread");
                
                data::forums c;
                prepare_content(c,page);

                cache().store_page(key);

        }
        
}


} // namespace apps


