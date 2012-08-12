#ifndef APPS_THREAD_H
#define APPS_THREAD_H
#include <cppcms/application.h>
#include <string>
#include <apps/master.h>
#include <data/thread.h>

namespace apps {

class thread_shared : public master
{
public:
        thread_shared(cppcms::service &s);
        bool prepare(data::thread_shared &c,int id);

};

class user_thread : public thread_shared {
public:

        user_thread(cppcms::service &s);
        void prepare(std::string id); 
};

class adm_thread : public thread_shared {
public:
        adm_thread(cppcms::service &s);
        void prepare(std::string id);
};

class auth : public thread_shared {
public:
        auth(cppcms::service &s);
        void prepare(std::string id);
};


} // apps


#endif
