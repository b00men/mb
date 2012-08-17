#include "forums.h"
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include <cppcms/cache_interface.h>
#include <boost/lexical_cast.hpp>
#include <cppcms/http_file.h>
#include <Magick++.h>
#include <cppcms/json.h>
#include <apps/addfunc.h>

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
    file.message(translate("Upload"));
    file.limits(0,5*1024*1024);
    submit.value(translate("Create"));
    add(title);
    add(author);
    add(comment);
    add(file);
    add(submit);
}

delete_thrds_form::delete_thrds_form()
{
    using cppcms::locale::translate;
    submit.name("delete");
    submit.value(translate("Delete"));
    submit.message(translate("Delete selected threads"));   
    add(submit);
    add(checkboxes);
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
    data::forums c;
    c.is_admin=((session().is_set("login"))&&(session()["login"]=="true"))?(1):(0);
    if(request().request_method()=="POST") {

        c.form.load(context());
        if(c.form.validate()) {
            cppdb::transaction tr(sql);
            cppdb::statement st;
            cppdb::statement nextst;
            std::string path="";
			std::string path_thumb="";
        	if(c.form.file.set())
            {
                std::string filetype=c.form.file.value()->mime();
                filedetect(path, path_thumb, filetype); //addfunc
            }
            st= sql <<"INSERT INTO threads(title) VALUES(?)" 
                    << c.form.title.value() << cppdb::exec;
            int tid=st.last_insert_id();
            nextst= sql<<   "INSERT INTO messages(thread_id,file,reply_to,content,author) "
                            "VALUES (?,?,0,?,?)"
                    << tid << path << c.form.comment.value() << c.form.author.value() << cppdb::exec;
            int id=nextst.last_insert_id();
            if(c.form.file.set())
            {   
            	std::stringstream ss;
            	ss<<settings().get<std::string>("mb.uploads")<<tid<<"_"<<id<<"."<<path;
        		path="";
            	ss>>path;
			    c.form.file.value()->save_to(path);
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

            response().set_redirect_header(url("/thread",tid));
            c.form.clear();
            return;
        }

        if (c.is_admin){
            // 1) Читаем возможные треды. Готовим change
            const unsigned topics_per_page=10;
            int offset= page.empty() ? 0 : atoi(page.c_str());
            cppdb::result r;
            r=sql<< "SELECT id "
                    "FROM threads "
                    "ORDER BY date DESC "
                    "LIMIT ?,?" << offset*topics_per_page << topics_per_page;
            std::string sthrd_id="";
            int size_thr=0;
            bool change=0;

            // 2) Цикл: каждому треду по чекбоксу.
            c.dthr_form.boxes.reserve(10);
            for(int i=0;r.next();i++) {
                r>>sthrd_id;
                c.dthr_form.boxes.resize(i+1);
                cppcms::widgets::checkbox *box = new cppcms::widgets::checkbox();
                c.dthr_form.checkboxes.attach(box); // transfer ownership and register to parent
                box->identification(sthrd_id);
                box->name(sthrd_id);
                c.dthr_form.boxes.push_back(box);
                size_thr=i+1;                       
            }

            // 3) Грузим контент.
            c.dthr_form.load(context());

            std::string _id="";
            // 4) Цикл: работа с каждым чекбоксом:
            for(int i=1; i<size_thr+1; ++i){
            //     4.1) if: Снимаем значение с чекбокса:
                if (c.dthr_form.boxes[i]->value()) {
                    change=1;

            //         4.1.1) Копируем почти полностью (без onlyfile)
            //                работу по удалению треда из thread.cpp
                    std::string smeg_id="";
                    std::string smeg_file="";
                    cppdb::result r;
                    cppdb::result rthr;
                    rthr=sql<< "SELECT id,file "
                            "FROM messages WHERE thread_id=? "
                            "ORDER BY id" << c.dthr_form.boxes[i]->name();

                    for(int j=0;rthr.next();j++) {
                        rthr>>smeg_id>>smeg_file;

                        sql<<   "DELETE "
                                "FROM messages "
                                "WHERE id=? " 
                                << smeg_id << cppdb::exec;
                        if (smeg_file.length()) {
                            std::stringstream ss;
                            std::stringstream ss2;
                            std::string delpath="";
                            ss<<settings().get<std::string>("mb.uploads")<<c.dthr_form.boxes[i]->name()<<"_"<<smeg_id<<"."<<smeg_file;
                            ss>>delpath;
                            std::remove(delpath.c_str());
                            ss2<<settings().get<std::string>("mb.uploads")<<"thumb_"<<c.dthr_form.boxes[i]->name()<<"_"<<smeg_id<<"."<<smeg_file;
                            delpath="";
                            ss2>>delpath;
                            std::remove(delpath.c_str());
                        }
                    }

                    cache().rise("thread_" + c.dthr_form.boxes[i]->name());
                    cache().rise("remove_thread");
                    sql<<   "DELETE "
                            "FROM threads "
                            "WHERE id=? " 
                            << c.dthr_form.boxes[i]->name() << cppdb::exec;
                }
            }
            // 5) Если работа проводилась - обновить кеш.
            if(change) {
                cache().rise("new_thread");
                c.form.clear();
                response().set_redirect_header(url("/forums",page));

                return;
            }
        }
    }
    std::string key;
    if (c.is_admin){
        key = "admin_forums_" + page;
    }
    else{
        key = "user_forums_" + page;
    }
    if(cache().fetch_page(key))
            return;
    // Add some shared key for all main_page_
    cache().add_trigger("new_thread");
    
    prepare_content(c,page);

    cache().store_page(key);
        
}

} // namespace apps


