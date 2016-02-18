#include "webserver_model.h"
#include "webserver_view.h"

#include <cppcms/application.h>  
#include <cppcms/applications_pool.h>  
#include <cppcms/service.h>  
#include <cppcms/http_response.h>  
#include <iostream>
#include <string>

void webserver_view::main(std::string url) {
  content::message c;

  auto number = std::to_string(aggregate_debug_info_count());
  auto rate = std::to_string(aggregate_debug_info_rate_per_minute());
  c.num_messages = number;
  c.data_rate = rate;

  render("message", c);
} 
