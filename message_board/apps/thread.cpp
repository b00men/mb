#include <apps/thread.h>
#include <cppcms/util.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include <cppcms/cache_interface.h>
#include <cppcms/http_file.h>
#include <markdown/markdown.h>


namespace data {

reply_form::reply_form()
{
        using cppcms::locale::translate;
        author.message(translate("Author"));
        author.value(translate("Anon"));
        author.error_message(translate("Error: empty author."));
        comment.message(translate("Comment"));
        comment.rows(6);
        comment.cols(60);
        comment.error_message(translate("Error: empty message."));
        image.message(translate("Upload"));
        image.limits(0,5*1024*1024);
        send.value(translate("Send"));
        add(author);
        add(comment);
        add(image);
        add(send);
        author.limits(1,64);
        //comment.limits(1,8192);
        comment.non_empty();
}


} // data

namespace {
        std::string text2html(std::string const &s)
        {
				std::string res=s;
                /*res.reserve(s.size());
                for(unsigned i=0;i<s.size();i++) {
                        if(s[i]=='<') {
		                	if((s[i+1]=='h')&&(s[i+2]=='t')) {
		                    res+="<";
		                    }
                        	else res+="<!";
                        }
                        else res+=s[i];
                }*/
				markdown::Document doc;
				std::stringstream ss;
				doc.read(res);
				doc.write(ss);	
				res = ss.str();
                return res;
        }
}


namespace apps {

thread_shared::thread_shared(cppcms::service &s) : apps::master(s)
{
}


bool thread_shared::prepare(data::thread_shared &c,int id)
{
        master::prepare(c);
        c.thread_id = id;
        

        cppdb::result r;
        r=sql<<"SELECT title FROM threads WHERE id=?" << id << cppdb::row;
        if(r.empty()) {
                response().make_error_response(404);
                return false;
        }
        
        r>> c.title;
        c.text2html = text2html;
        return true;

}

flat_thread::flat_thread(cppcms::service &s) : thread_shared(s)
{
        dispatcher().assign(".*",&flat_thread::prepare,this,0);
        mapper().assign("{1}");
}


void flat_thread::prepare(std::string sid) // sid (id) - id of thread
{
        std::string key = "flat_thread_" + sid;
        int mid = 0;

        cache().add_trigger("thread_" + sid);
        
        data::flat_thread c;
        data::reply c2;
        
        int id = atoi(sid.c_str());
        if(request().request_method()=="POST") {
                c2.form.load(context());
                if(c2.form.validate()) {
                        cppdb::transaction tr(sql);
                        cppdb::statement st;
                        
						std::string path="";
						
                    	if(c2.form.image.set())
                        {
		                	path = "txt";
		                	if(c2.form.image.value()->mime() == "image/png") path="png";
		                	if(c2.form.image.value()->mime() == "image/jpeg") path="jpg";
		                	if(c2.form.image.value()->mime() == "image/gif") path="gif";
		                	if(c2.form.image.value()->mime() == "image/svg+xml") path="svg";
		                	if(c2.form.image.value()->mime() == "audio/mpeg") path="mp3";
		                	if(c2.form.image.value()->mime() == "audio/ogg") path="ogg";
		                	if(c2.form.image.value()->mime() == "audio/vnd.wave") path="wav";
		                	if(c2.form.image.value()->mime() == "text/css") path="css";
		                	if(c2.form.image.value()->mime() == "text/csv") path="csv";
		                	if(c2.form.image.value()->mime() == "text/html") path="html";
		                	if(c2.form.image.value()->mime() == "text/javascript") path="js";
		                	if(c2.form.image.value()->mime() == "text/xml") path="xml";
		                	if(c2.form.image.value()->mime() == "video/mpeg") path="mpg";
		                	if(c2.form.image.value()->mime() == "video/mp4") path="mp4";
		                	if(c2.form.image.value()->mime() == "video/ogg") path="ogg";
		                	if(c2.form.image.value()->mime() == "video/webm") path="webm";
		                	if(c2.form.image.value()->mime() == "video/x-matroska") path="mkv";
		                	if(c2.form.image.value()->mime() == "video/x-ms-wmv") path="wmv";
		                	if(c2.form.image.value()->mime() == "video/x-flv") path="flv";
		                	if(c2.form.image.value()->mime() == "application/pdf") path="pdf";
		                	if(c2.form.image.value()->mime() == "application/zip") path="zip";
		                	if(c2.form.image.value()->mime() == "application/x-gzip") path="gz";
		                	if(c2.form.image.value()->mime() == "application/x-tar") path="tar";
		                	if(c2.form.image.value()->mime() == "application/x-deb") path="deb";
		                	if(c2.form.image.value()->mime() == "application/x-rar") path="rar";
                        }
                        
                        st=sql<<   "INSERT INTO messages(reply_to,thread_id,author,content,file) "
                                "VALUES(?,?,?,?,?) " 
                                << mid << id << c2.form.author.value() << c2.form.comment.value() 
                                << path << cppdb::exec;
                                
                        int lastid=st.last_insert_id();       
                        if(c2.form.image.set())
                        {
                        	std::stringstream ss;
                        	ss<<"./media/uploads/"<<id<<"_"<<lastid<<"."<<path;
                    		path="";
                        	ss>>path;
						    c2.form.image.value()->save_to(path);
                        }

                        cache().rise("thread_" + sid);
                        tr.commit();

                        response().set_redirect_header(url("/user_thread",id));
                        c2.form.clear();
                        return;
                }        
                c2.thread_id = id;
				c2.msg_id = mid;
				
				if(!thread_shared::prepare(c2,id))
				        return;

				render("reply",c2);
        }
        
        else{
		
		if(cache().fetch_page(key))
		        return;

		if(!thread_shared::prepare(c,id))
		        return;  
		             
		        
		cppdb::result r;
		r=sql<< "SELECT id,thread_id,author,content,reply_to,date,file "
		        "FROM messages WHERE thread_id=? "
		        "ORDER BY id" << id;
		
		c.messages.reserve(10);
		for(int i=0;r.next();i++) {
		        c.messages.resize(i+1);
		        r>>c.messages[i].msg_id>>c.messages[i].tid>>c.messages[i].author>>c.messages[i].content>>c.messages[i].reply_to>>c.messages[i].date>>c.messages[i].file;
		}
		session()["view"]="flat";
		render("flat_thread",c);

		cache().store_page(key);
		}
}

typedef std::map<int,std::map<int,data::msg> > msg_ord_t;

namespace {

void make_tree(data::tree_t &messages,std::map<int,std::map<int,data::msg> > &content,int start)
{
        std::pair<msg_ord_t::iterator,msg_ord_t::iterator>
                range=content.equal_range(start);
        for(msg_ord_t::iterator p=range.first;p!=range.second;++p) {
                for(std::map<int,data::msg>::iterator p2=p->second.begin(),e=p->second.end();p2!=e;++p2) {
                        data::tree_thread::tree_msg &m=messages[p2->first];
                        m.author=p2->second.author;
                        m.content=p2->second.content;
                        m.msg_id=p2->second.msg_id;
                        m.date=p2->second.date;
                        m.file=p2->second.file;
                        m.tid=p2->second.tid;
                        make_tree(m.repl,content,p2->first);
                }
        }
        
}

} // anon

tree_thread::tree_thread(cppcms::service &s) : thread_shared(s)
{
        dispatcher().assign(".*",&tree_thread::prepare,this,0);
        mapper().assign("{1}");
}



void tree_thread::prepare(std::string sid)
{
        std::string key = "tree_thread_" + sid;
        
        if(cache().fetch_page(key))
                return;

        cache().add_trigger("thread_" + sid);

        int id = atoi(sid.c_str());
        data::tree_thread c;
        if(!thread_shared::prepare(c,id))
                return;
        cppdb::result r;
        r=sql<< "SELECT reply_to,id,thread_id,author,content,file,date "
                "FROM messages WHERE thread_id=? "
                "ORDER BY reply_to,id DESC" << id;
        msg_ord_t all;
        while(r.next()) {
                int msg_id,rpl_id,tid;
                r>>rpl_id>>msg_id>>tid;
                data::msg &message=all[rpl_id][msg_id];
                r>>message.author>>message.content>>message.file>>message.date;
                message.msg_id=msg_id;
                message.tid=tid;
        }
        
        make_tree(c.messages,all,0);

        session()["view"]="tree";
        render("tree_thread",c);

        cache().store_page(key);
}

reply::reply(cppcms::service &srv) : thread_shared(srv)
{
        dispatcher().assign(".*",&reply::prepare,this,0);
        mapper().assign("{1}");
}

void reply::prepare(std::string smid) // smid (mid) - id of message
{
        int mid;
        mid=atoi(smid.c_str());

        data::reply c;
        
        if(request().request_method()=="POST") {
                c.form.load(context());
                if(c.form.validate()) {
                        cppdb::transaction tr(sql);
                        cppdb::result r;
                        cppdb::statement st;
                        
                        r=sql<<"SELECT thread_id FROM messages WHERE id=?" << mid << cppdb::row;
                        if(r.empty()) {
                                response().make_error_response(404);
                                return;
                        }
                        int tid = r.get<int>(0);
						std::string path="";
                    	if(c.form.image.set())
                        {
		                	path = "txt";
		                	if(c.form.image.value()->mime() == "image/png") path="png";
		                	if(c.form.image.value()->mime() == "image/jpeg") path="jpg";
		                	if(c.form.image.value()->mime() == "image/gif") path="gif";
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
                        
                        // we want this as string for trigger
                        std::string stid = r.get<std::string>(0);
                        r.clear();
                        st=sql<<   "INSERT INTO messages(reply_to,thread_id,author,content,file) "
                                "VALUES(?,?,?,?,?) " 
                                << mid << tid << c.form.author.value() << c.form.comment.value() 
                                << path << cppdb::exec;
                                
                        int id=st.last_insert_id();       
                        if(c.form.image.set())
                        {   
                        	//std::string new_name = c.form.image.value()->filename();
                        	std::stringstream ss;
                        	ss<<"./media/uploads/"<<tid<<"_"<<id<<"."<<path;
                    		path="";
                        	ss>>path;
						    c.form.image.value()->save_to(path);
                        }

                        cache().rise("thread_" + stid);
                        tr.commit();

                        response().set_redirect_header(url("/user_thread",tid));
                        c.form.clear();
                        return;
                }
        }

        cppdb::result r;
        r=sql<< "SELECT threads.id,author,content,title "
                "FROM messages "
                "JOIN threads ON thread_id=threads.id "
                "WHERE messages.id=?" << mid << cppdb::row;
        if(r.empty()) {
                response().make_error_response(404);
                return;
        }

        int tid;

        r>>tid>>c.author>>c.content>>c.title;
        c.thread_id = tid;
        c.msg_id = mid;
        
        if(!thread_shared::prepare(c,tid))
                return;

        render("reply",c);
}

} // namespace apps


