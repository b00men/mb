#include <apps/auth.h>
#include <cppcms/util.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include <cppcms/cache_interface.h>
#include <cppcms/http_file.h>
#include <markdown/markdown.h>
#include <Magick++.h>
#include <cppcms/json.h>


namespace data {

login_form::login_form()
{
        using cppcms::locale::translate;
        login.message(translate("Login"));
        password.message(translate("Password"));
        submit.value(translate("Sign in"));
        add(login);
        add(password);
        add(submit);
        login.non_empty();
        password.non_empty();
}

} // data

namespace apps {

auth::auth(cppcms::service &s) : apps::master(s)
{
        mapper().assign("");
        dispatcher().assign(".*",&auth::prepare,this,0);
}

void auth::prepare(std::string smid)
{
		data::auth c;
		using cppcms::locale::translate;
		c.status=translate("You are not authentication.");
		if(request().request_method()=="POST") {
				c.form.load(context());
				if(c.form.validate())
				if ((c.form.login.value()==settings().get<std::string>("mb.admin_login"))&&(c.form.password.value()==settings().get<std::string>("mb.admin_password"))) {
					session().reset_session();
					session()["login"]="true";
				}
				else {
				c.status=translate("Login incorect!");
				session().clear();
				}
		}
		if ((session().is_set("login"))&&(session()["login"]=="true"))
			c.status=translate("Login success!");

		master::prepare(c);
		render("auth",c);		
}

} // namespace apps