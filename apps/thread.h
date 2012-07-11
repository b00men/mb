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

class flat_thread : public thread_shared {
public:

        flat_thread(cppcms::service &s);
        void prepare(std::string id); 
};

class auth : public thread_shared {
public:

        auth(cppcms::service &s);
        void prepare(std::string id);
};


} // apps


#endif
