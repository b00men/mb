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

        attach( new adm_forums(s),
                "adm_forums",
                "/adm_forums/{1}",
                "(/adm_forums/(\\d+)?)?",2);

        attach( new user_thread(s),
                "user_thread",
                "/thread/{1}",
                "/thread/(\\d+)",1);

        attach( new adm_thread(s),
                "adm_thread",
                "/adm_thread/{1}",
                "/adm_thread/(\\d+)",1);

        attach( new auth(s),
                "admin",
                "/admin/{1}",
                "/admin(/)?",2);
        
        // Generic mapping
        mapper().root(settings().get<std::string>("mb.root"));
}

} // apps


