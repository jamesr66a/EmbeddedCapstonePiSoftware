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
  auto vec = aggregate_info_vector();

  c.num_messages = number;
  c.data_rate = rate;
  c.message_list =
      "<div class = \"container\"><table class = \"table table-striped\">";
  c.message_list += "<thead><tr><th>Identifier</th><th>Debug "
                    "ID</th><th>Data</th></tr></thead><tbody>";
  for (const auto &x : vec) {
    c.message_list += "<tr><td>" + std::to_string(DebugInfo_identifier(&x)) +
                      "</td><td>" + std::to_string(DebugInfo_debugID(&x)) +
                      "</td><td>" + std::to_string(DebugInfo_data(&x)) +
                      "</td></tr>";
  }
  c.message_list += "</tbody></table></div>";

  render("message", c);
}
