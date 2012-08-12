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


namespace data {

reply_form::reply_form()
{
        using cppcms::locale::translate;
        author.message(translate("Author"));
        author.value(translate("Anon"));
        comment.message(translate("Message"));
        comment.attributes_string(" cols=55 rows=6 onkeypress='key2post(event, this.form);' ");
        image.message(translate("Upload"));
        image.limits(0,5*1024*1024);
        send.value(translate("Send"));
        add(author);
        add(comment);
        add(image);
        add(send);
        author.limits(1,64);
        comment.non_empty();
}

login_form::login_form()
{
        using cppcms::locale::translate;
        login.message(translate("Login"));
        password.message(translate("Password"));
        submit.value(translate("Sign in"));
        add(login);
        add(password);
        add(submit);
        login.non_empty();
        password.non_empty();
}

delete_msg_form::delete_msg_form()
{
        using cppcms::locale::translate;
        submit.value(translate("Delete"));
        submit.message(translate("Delete selected messages"));
        onlyfile.message(translate("Delete ONLY file(s)"));  
        onlyfile.name("onlyfile");       	
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

user_thread::user_thread(cppcms::service &s) : thread_shared(s)
{
        dispatcher().assign(".*",&user_thread::prepare,this,0);
        mapper().assign("{1}");
}


void user_thread::prepare(std::string sid) // sid (id) - id of thread
{
		data::user_thread c;
        int mid = 0;
        int id = atoi(sid.c_str());
        if(request().request_method()=="POST") {
                c.form.load(context());
                if(c.form.validate()) {
                        cppdb::transaction tr(sql);
                        cppdb::statement st;
                        
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
                        
                        st=sql<<   "INSERT INTO messages(reply_to,thread_id,author,content,file) "
                                "VALUES(?,?,?,?,?) " 
                                << mid << id << c.form.author.value() << c.form.comment.value() 
                                << path << cppdb::exec;
                                
                        int lastid=st.last_insert_id();

                        st=sql<<   "UPDATE threads "
                                "SET date=NOW() "
                                "WHERE id=? " 
                                << id << cppdb::exec;
                                       
                        if(c.form.image.set())
                        {
                        	std::stringstream ss;
                        	ss<<settings().get<std::string>("mb.uploads")<<id<<"_"<<lastid<<"."<<path;
                    		path="";
                        	ss>>path;
						    c.form.image.value()->save_to(path);
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

                        response().set_redirect_header(url("/user_thread",id));
                        c.form.clear();
                        return;
                }
        }
		std::string key = "user_thread_" + sid;
		if(cache().fetch_page(key))
				return;
        cache().add_trigger("thread_" + sid);
		thread_shared::prepare(c,id);    
		        
		cppdb::result r;
		r=sql<< "SELECT id,thread_id,author,content,reply_to,date,file,thumb "
		        "FROM messages WHERE thread_id=? "
		        "ORDER BY id" << id;
		
		c.messages.reserve(10);
		for(int i=0;r.next();i++) {
		        c.messages.resize(i+1);
		        r>>c.messages[i].msg_id>>c.messages[i].tid>>c.messages[i].author>>c.messages[i].content>>c.messages[i].reply_to>>c.messages[i].date>>c.messages[i].file>>c.messages[i].thumb;
		}		

		
		render("user_thread",c);
		cache().store_page(key);		
}

adm_thread::adm_thread(cppcms::service &s) : thread_shared(s)
{
        dispatcher().assign(".*",&adm_thread::prepare,this,0);
        mapper().assign("{1}");
}


void adm_thread::prepare(std::string sid) // sid (id) - id of thread
{
		data::adm_thread c;
        int mid = 0;
        int id = atoi(sid.c_str());
		c.is_admin=((session().is_set("login"))&&(session()["login"]=="true"))?(1):(0);
		if (!c.is_admin)
		{
            response().make_error_response(404);
            return;
		}
        	if(request().request_method()=="POST") {

                c.form.load(context());
                if(c.form.validate()) {
                    cppdb::transaction tr(sql);
                    cppdb::statement st;
                    
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
                    
                    st=sql<<   "INSERT INTO messages(reply_to,thread_id,author,content,file) "
                            "VALUES(?,?,?,?,?) " 
                            << mid << id << c.form.author.value() << c.form.comment.value() 
                            << path << cppdb::exec;
                            
                    int lastid=st.last_insert_id();

                    st=sql<<   "UPDATE threads "
                            "SET date=NOW() "
                            "WHERE id=? " 
                            << id << cppdb::exec;
                                   
                    if(c.form.image.set())
                    {
                    	std::stringstream ss;
                    	ss<<settings().get<std::string>("mb.uploads")<<id<<"_"<<lastid<<"."<<path;
                		path="";
                    	ss>>path;
					    c.form.image.value()->save_to(path);
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

                    response().set_redirect_header(url("/adm_thread",id));
                    c.form.clear();
                    return;
                }


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
	                cache().rise("remove_thread");

	                response().set_redirect_header(url("/adm_thread",id));
	                c.form.clear();
	                return;
                }
        }
		std::string key = "adm_thread_" + sid;
		if(cache().fetch_page(key))
				return;
        cache().add_trigger("thread_" + sid);
		thread_shared::prepare(c,id);    
		        
		cppdb::result r;
		r=sql<< "SELECT id,thread_id,author,content,reply_to,date,file,thumb "
		        "FROM messages WHERE thread_id=? "
		        "ORDER BY id" << id;
		
		c.messages.reserve(10);
		for(int i=0;r.next();i++) {
		        c.messages.resize(i+1);
		        r>>c.messages[i].msg_id>>c.messages[i].tid>>c.messages[i].author>>c.messages[i].content>>c.messages[i].reply_to>>c.messages[i].date>>c.messages[i].file>>c.messages[i].thumb;
		}		

		
		render("adm_thread",c);
		cache().store_page(key);		
}


auth::auth(cppcms::service &srv) : thread_shared(srv)
{
        mapper().assign("");
        dispatcher().assign(".*",&auth::prepare,this,0);
}

void auth::prepare(std::string smid)
{
		data::auth c;
		using cppcms::locale::translate;
		c.status=translate("You are not authentication.");
		if(request().request_method()=="POST") {
				c.form.load(context());
				if(c.form.validate())
				if ((c.form.login.value()==settings().get<std::string>("mb.admin_login"))&&(c.form.password.value()==settings().get<std::string>("mb.admin_password"))) {
					session().reset_session();
					session()["login"]="true";
				}
				else {
				c.status=translate("Login incorect!");
				session().clear();
				}
		}
		if ((session().is_set("login"))&&(session()["login"]=="true"))
			c.status=translate("Login success!");

		master::prepare(c);
		render("auth",c);		
}

} // namespace apps

