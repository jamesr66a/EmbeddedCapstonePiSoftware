#line 1 "webserver_view.tmpl"
#include "webserver_view.h" 
#line 2 "webserver_view.tmpl"
namespace my_skin {
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
			#line 7 "webserver_view.tmpl"
			out()<<"\n"
				"<html>\n"
				"  <body>\n"
				"    <h1>";
			#line 7 "webserver_view.tmpl"
			out()<<cppcms::filters::escape(content.num_messages);
			#line 7 "webserver_view.tmpl"
			out()<<" ";
			#line 7 "webserver_view.tmpl"
			out()<<cppcms::filters::escape(content.data_rate);
			#line 10 "webserver_view.tmpl"
			out()<<"</h1>\n"
				"  </body>\n"
				"</html>\n"
				"";
		#line 10 "webserver_view.tmpl"
		} // end of template render
	#line 11 "webserver_view.tmpl"
	}; // end of class message
#line 12 "webserver_view.tmpl"
} // end of namespace my_skin
#line 13 "webserver_view.tmpl"
namespace {
#line 13 "webserver_view.tmpl"
 cppcms::views::generator my_generator; 
#line 13 "webserver_view.tmpl"
 struct loader { 
#line 13 "webserver_view.tmpl"
  loader() { 
#line 13 "webserver_view.tmpl"
   my_generator.name("my_skin");
#line 13 "webserver_view.tmpl"
   my_generator.add_view<my_skin::message,content::message>("message",true);
#line 13 "webserver_view.tmpl"
    cppcms::views::pool::instance().add(my_generator);
#line 13 "webserver_view.tmpl"
 }
#line 13 "webserver_view.tmpl"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 13 "webserver_view.tmpl"
} a_loader;
#line 13 "webserver_view.tmpl"
} // anon 
