#include <apps/mb.h>
//#include <apps/filedet.h>
#include <apps/forums.h>
#include <apps/thread.h>
#include <apps/auth.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/json.h>


namespace apps {


mb::mb(cppcms::service &s) : cppcms::application(s)
{

        attach( new forums(s),
                "forums",
                "/{1}",
                "(/(\\d+)?)?",2);

        attach( new thread(s),
                "thread",
                "/thread/{1}",
                "/thread/(\\d+)",1);

        attach( new auth(s),
                "admin",
                "/admin/{1}",
                "/admin(/)?",2);
        
        // Generic mapping
        mapper().root(settings().get<std::string>("mb.root"));
}

} // apps


