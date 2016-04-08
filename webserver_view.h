#ifndef _WEBSERVER_VIEW_H_
#define _WEBSERVER_VIEW_H_

#include <cppcms/view.h>
#include <string>

namespace content {
struct message : public cppcms::base_content {
  std::string num_messages;
  std::string data_rate;
  std::string message_list;
};
}

class webserver_view : public cppcms::application {
public:
  webserver_view(cppcms::service &srv) : cppcms::application(srv) {}
  virtual void main(std::string url);
};

#endif /* _WEBSERVER_VIEW_H_ */
