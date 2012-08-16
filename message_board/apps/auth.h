#ifndef APPS_AUTH_H
#define APPS_AUTH_H
#include <cppcms/application.h>
#include <string>
#include <apps/master.h>
#include <data/auth.h>

namespace apps {

class auth : public master {
public:
        auth(cppcms::service &s);
        void prepare(std::string id);
};


} // apps


#endif