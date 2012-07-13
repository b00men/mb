#ifndef DATA_THREAD_H
#define DATA_THREAD_H
#include <string>

#include <data/master.h>
#include <booster/function.h>

namespace data {

struct msg {
        std::string author;
        std::string content;
        std::string reply_to;
        std::string date;
        std::string file;
        int thumb;
        int msg_id;
        int tid;
        msg() : msg_id(0) {}
};

struct reply_form : public cppcms::form {
        cppcms::widgets::text author;
        cppcms::widgets::textarea comment;
        cppcms::widgets::file image;
        cppcms::widgets::submit send;
        reply_form();
};

struct login_form : public cppcms::form {
        cppcms::widgets::text login;
        cppcms::widgets::password password;
        cppcms::widgets::submit submit;
        login_form();
};

struct delete_msg_form : public cppcms::form{
        cppcms::widgets::checkbox id_box;
        cppcms::widgets::submit del_msg;
        delete_msg_form();
};

//struct msgs : public msg, public delete_msg_form {
  //      msgs() {}
//};

class thread_shared : public master
{
public:
        thread_shared() 
        {
                thread_id = 0;
        }
        int thread_id;
        std::string title;
        std::string (*text2html)(std::string const &);
};

class flat_thread : public thread_shared , public msg {
public:
        std::vector<msg> messages;
        reply_form form;
};

class auth : public thread_shared{
public:
        login_form form;
        std::string status;
};


} // data


#endif


