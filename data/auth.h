#ifndef DATA_AUTH_H
#define DATA_AUTH_H
#include <string>

#include <data/master.h>
#include <booster/function.h>

namespace data {


struct login_form : public cppcms::form {
        cppcms::widgets::text login;
        cppcms::widgets::password password;
        cppcms::widgets::submit submit;
        login_form();
};

class auth : public master {
public:
        login_form form;
        std::string status;
};

} // data


#endif