#line 1 "webserver_view.tmpl"
#include "webserver_view.h" 
#line 2 "webserver_view.tmpl"
namespace webserver_template {
	#line 3 "webserver_view.tmpl"
	struct message :public cppcms::base_view
	#line 3 "webserver_view.tmpl"
	{
	#line 3 "webserver_view.tmpl"
		content::message &content;
	#line 3 "webserver_view.tmpl"
		message(std::ostream &_s,content::message &_content): cppcms::base_view(_s),content(_content)
	#line 3 "webserver_view.tmpl"
		{
	#line 3 "webserver_view.tmpl"
		}
		#line 4 "webserver_view.tmpl"
		virtual void render() {
			#line 25 "webserver_view.tmpl"
			out()<<"\n"
				"<!DOCTYPE html>\n"
				"<html lang=\"en\">\n"
				"  <head>\n"
				"    <meta charset=\"utf-8\">\n"
				"    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
				"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
				"    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->\n"
				"    <title>Team 16 Dashboard</title>\n"
				"\n"
				"    <!-- Bootstrap -->\n"
				"    <link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" rel=\"stylesheet\">\n"
				"\n"
				"    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->\n"
				"    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->\n"
				"    <!--[if lt IE 9]>\n"
				"      <script src=\"https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js\"></script>\n"
				"      <script src=\"https://oss.maxcdn.com/respond/1.4.2/respond.min.js\"></script>\n"
				"    <![endif]-->\n"
				"  </head>\n"
				"  <body><center>\n"
				"    <h1><span class=\"label label-success\">Total number of messages:";
			#line 25 "webserver_view.tmpl"
			out()<<cppcms::filters::escape(content.num_messages);
			#line 25 "webserver_view.tmpl"
			out()<<"</span><span class=\"label label-success\">Message rate (msgs/minute):";
			#line 25 "webserver_view.tmpl"
			out()<<cppcms::filters::escape(content.data_rate);
			#line 26 "webserver_view.tmpl"
			out()<<"</span></h1><br/>\n"
				"    ";
			#line 26 "webserver_view.tmpl"
			out()<<cppcms::filters::raw(content.message_list);
			#line 34 "webserver_view.tmpl"
			out()<<"\n"
				"    </center>\n"
				"    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->\n"
				"    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\n"
				"    <!-- Include all compiled plugins (below), or include individual files as needed -->\n"
				"    <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n"
				"  </body>\n"
				"</html>\n"
				"";
		#line 34 "webserver_view.tmpl"
		} // end of template render
	#line 35 "webserver_view.tmpl"
	}; // end of class message
#line 36 "webserver_view.tmpl"
} // end of namespace webserver_template
#line 37 "webserver_view.tmpl"
namespace {
#line 37 "webserver_view.tmpl"
 cppcms::views::generator my_generator; 
#line 37 "webserver_view.tmpl"
 struct loader { 
#line 37 "webserver_view.tmpl"
  loader() { 
#line 37 "webserver_view.tmpl"
   my_generator.name("webserver_template");
#line 37 "webserver_view.tmpl"
   my_generator.add_view<webserver_template::message,content::message>("message",true);
#line 37 "webserver_view.tmpl"
    cppcms::views::pool::instance().add(my_generator);
#line 37 "webserver_view.tmpl"
 }
#line 37 "webserver_view.tmpl"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 37 "webserver_view.tmpl"
} a_loader;
#line 37 "webserver_view.tmpl"
} // anon 
