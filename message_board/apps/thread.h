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

class thread : public thread_shared {
public:
        thread(cppcms::service &s);
        void prepare(std::string id);
};

} // apps


#endif
