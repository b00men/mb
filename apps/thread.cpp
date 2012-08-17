#include <apps/thread.h>
#include <cppcms/util.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include <cppcms/cache_interface.h>
#include <cppcms/http_file.h>
#include <markdown/markdown.h>
#include <Magick++.h>
#include <cppcms/json.h>
#include <apps/addfunc.h>


namespace data {

reply_form::reply_form()
{
    using cppcms::locale::translate;
    author.message(translate("Author"));
    author.value(translate("Anon"));
    comment.message(translate("Message"));
    comment.attributes_string(" cols=55 rows=6 onkeypress='key2post(event, this.form);' ");
    file.message(translate("Upload"));
    file.limits(0,5*1024*1024);
    send.value(translate("Send"));
    add(author);
    add(comment);
    add(file);
    add(send);
    author.limits(1,64);
    comment.non_empty();
}

delete_msg_form::delete_msg_form()
{
    using cppcms::locale::translate;
    submit.value(translate("Delete"));
    submit.message(translate("Delete selected messages"));
    onlyfile.message(translate("Delete ONLY file(s)"));  
    onlyfile.name("onlyfile");  
    onlyfile.value(translate("Delete"));     	
	add(submit);
	add(checkboxes);
	add(onlyfile);
}

} // data


namespace {

std::string text2html(std::string const &s)
{
	std::string out;
	markdown::Document doc;
	std::stringstream ss;
	doc.read(s);
	doc.write(ss);	
	out = ss.str();
    return out;
}

} // anon


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


thread::thread(cppcms::service &s) : thread_shared(s)
{
    dispatcher().assign(".*",&thread::prepare,this,0);
    mapper().assign("{1}");
}


void thread::prepare(std::string sid) // sid (id) - id of thread
{
	data::thread c;
    int mid = 0;
    int id = atoi(sid.c_str());
	c.is_admin=((session().is_set("login"))&&(session()["login"]=="true"))?(1):(0);
	
	if(request().request_method()=="POST") {
        c.form.load(context());
        if(c.form.validate()) {
            cppdb::transaction tr(sql);
            cppdb::statement st;
            
			std::string path="";
			std::string path_thumb="";
			
        	if(c.form.file.set())
            {
                	std::string filetype=c.form.file.value()->mime();
					filedetect(path, path_thumb, filetype); //addfunc
            }
            
            st=sql<<   "INSERT INTO messages(reply_to,thread_id,author,content,file) "
                       "VALUES(?,?,?,?,?) " 
                       << mid << id << c.form.author.value() << c.form.comment.value() 
                       << path << cppdb::exec;
                    
            int lastid=st.last_insert_id();

            st=sql<<   "UPDATE threads "
                       "SET date=NOW() "
                       "WHERE id=? " 
                       << id << cppdb::exec;
                           
            if(c.form.file.set())
            {
            	std::stringstream ss;
            	ss<<settings().get<std::string>("mb.uploads")<<id<<"_"<<lastid<<"."<<path;
        		path="";
            	ss>>path;
			    c.form.file.value()->save_to(path);
            }

            if(path_thumb!="")
            {
            	std::stringstream ss2;
            	ss2<<settings().get<std::string>("mb.uploads")<<"thumb_"<<id<<"_"<<lastid<<"."<<path_thumb;
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
				                << lastid << cppdb::exec;
						}
					catch( std::exception &error_ )
						{
						std::cout<<"Error resize image! Where:\nInput image: "<<path<<"\nOutput image: "<<path_thumb;
						}
            }

            cache().rise("thread_" + sid);
            cache().rise("new_thread");
            tr.commit();

            response().set_redirect_header(url("/thread",id));
            c.form.clear();
            return;
        }

		if (c.is_admin){
			cppdb::result r;
			r=sql<< "SELECT id,file "
					"FROM messages WHERE thread_id=? "
					"ORDER BY id" << id;
					
			std::string smeg_id="";
			std::string smeg_file="";
			int size_mes=0;
			bool change=0;
			c.dmes_form.boxes.reserve(10);
			for(int i=0;r.next();i++) {
				r>>smeg_id>>smeg_file;
				c.dmes_form.boxes.resize(i+1);
				cppcms::widgets::checkbox *box = new cppcms::widgets::checkbox();
				c.dmes_form.checkboxes.attach(box); // transfer ownership and register to parent
				box->identification(sid);
				box->name(smeg_id);
				box->message(smeg_file);
				c.dmes_form.boxes.push_back(box);
				size_mes=i+1;						
			}
            c.dmes_form.load(context());			
			for(int i=1; i<size_mes+1; ++i){
				if (c.dmes_form.boxes[i]->value()) {
					change=1;
                    if (c.dmes_form.onlyfile.value())
                    {
						sql<<   "UPDATE messages "
						           "SET file='',thumb=0 "
						           "WHERE id=? " 
						           << c.dmes_form.boxes[i]->name() << cppdb::exec;
                    }
                    else
                    {
	                    sql<<   "DELETE "
	                    		"FROM messages "
	                            "WHERE id=? " 
	                            << c.dmes_form.boxes[i]->name() << cppdb::exec;
                    }
					if (smeg_file.length())
					{
						std::stringstream ss;
						std::stringstream ss2;
						std::string delpath="";
                		ss<<settings().get<std::string>("mb.uploads")<<sid<<"_"<<c.dmes_form.boxes[i]->name()<<"."<<c.dmes_form.boxes[i]->message();
						ss>>delpath;
						std::remove(delpath.c_str());
						ss2<<settings().get<std::string>("mb.uploads")<<"thumb_"<<sid<<"_"<<c.dmes_form.boxes[i]->name()<<"."<<c.dmes_form.boxes[i]->message();
						delpath="";
						ss2>>delpath;
						std::remove(delpath.c_str());
					}
				}
			}
			if(change) {
                cache().rise("thread_" + sid);
                cache().rise("remove_post");

                response().set_redirect_header(url("/thread",id));
                c.form.clear();
                return;
            }
        }
    }
    std::string key;
    if (c.is_admin){
		key = "admin_thread_" + sid;
	}
	else{
		key = "user_thread_" + sid;
	}
	if(cache().fetch_page(key))
			return;
    cache().add_trigger("thread_" + sid);
  
	        
	cppdb::result r;
	r=sql<< "SELECT id,thread_id,author,content,reply_to,date,file,thumb "
	        "FROM messages WHERE thread_id=? "
	        "ORDER BY id" << id;
	
	c.messages.reserve(10);
	for(int i=0;r.next();i++) {
        c.messages.resize(i+1);
        r>>c.messages[i].msg_id>>c.messages[i].tid>>c.messages[i].author>>c.messages[i].content>>c.messages[i].reply_to>>c.messages[i].date>>c.messages[i].file>>c.messages[i].thumb;
	}		
	if (thread_shared::prepare(c,id)) render("thread",c);
	cache().store_page(key);
}

} // namespace apps

