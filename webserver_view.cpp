#include "webserver_model.h"
#include "webserver_view.h"

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <iostream>
#include <string>

std::string task_names[] = { "Sensor 1", "RSSI Data Collector", "Encoder 1",
                             "Encoder 2", "Rover Pose Calculator",
                             "PID Controller", "Motor 1", "Motor 2" };

std::string event_names[8][2] = {
  { "Receive", "Send" },
  { "Receive" },
  { "Receive" },
  { "Receive" },
  { "Receive" },
  { "Receive" },
  { "Receive" },
  { "Receive" },
};

void webserver_view::main(std::string url) {
  content::message c;

  auto number = std::to_string(aggregate_debug_info_count());
  auto rate = std::to_string(aggregate_debug_info_rate_per_minute());
  auto vec = aggregate_info_vector();

  c.num_messages = number;
  c.data_rate = rate;
  c.message_list =
      "<div class = \"container\"><table class = \"table table-striped\">";
  c.message_list +=
      "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
      "ID</th><th>Data</th></tr></thead><tbody>";
  for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
    auto &x = *itr;
    c.message_list +=
        "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) + "<td>" +
        task_names[DebugInfo_identifier(&x)] + "</td><td>" +
        event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
        "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
  }
  c.message_list += "</tbody></table></div>";

  render("message", c);
}
