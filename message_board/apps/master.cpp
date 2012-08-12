#include "master.h"

#include <cppcms/json.h>
#include <cppcms/session_interface.h>
#include <cppcms/url_mapper.h>
#include <iostream>

namespace apps {

master::master(cppcms::service &srv) : cppcms::application(srv)
{       
        conn_str_ = settings().get<std::string>("mb.connection_string");
        media_=settings().get<std::string>("mb.media");
}

void master::init()
{
        sql.open(conn_str_);
}
void master::clear()
{
        sql.close();
}

void master::prepare(data::master &c)
{
        c.media=media_;
        c.url_mrkdwn=1;
}


} // namespace apps
