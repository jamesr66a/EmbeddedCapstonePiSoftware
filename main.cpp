#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <fcntl.h>
#include <gflags/gflags.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "astar_model.h"
#include "debug.h"
#include "encoders_model.h"
#include "errorcheck_model.h"
#include "generated/RoverPose.pbo.h"
#include "generated/RSSIData.pbo.h"
#include "motor_model.h"
#include "pid_model.h"
#include "pose_model.h"
#include "rssi_model.h"
#include "RSSIVectorConstructor.h"
#include "serial.h"
#include "sensors_model.h"
#include "uart_receiver.h"
#include "uart_transmitter.h"
#include "warning_model.h"
#include "webserver_model.h"
#include "webserver_view.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

DEFINE_string(uart_filename, "/dev/ttyUSB0",
              "Device file name for the UART device");
DEFINE_string(csv_filename, "/dev/null",
              "File name for the CSV file to write to");
DEFINE_string(bokeh_server_address, "", "Server address for the Bokeh server");
DEFINE_string(server_config, "config.js", "");

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Initialize receive device file
  int fd = open(FLAGS_uart_filename.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    perror("error opening");
    errorCheck();
  }
  set_interface_attribs(fd, B115200, 0);
  set_blocking(fd, 1);

  std::ofstream out_csv;
  out_csv.open(FLAGS_csv_filename.c_str(), std::ios_base::app);

  auto vector_cb = [&out_csv](
      const std::vector<std::tuple<RSSIData, RoverPose>> &vec) {
    double x = 0, y = 0, yaw = 0;
    for (auto &tup : vec) {
      auto &pose = std::get<1>(tup);
      x += double(RoverPose_xPosition(&pose)) / vec.size();
      y += double(RoverPose_yPosition(&pose)) / vec.size();
      yaw += double(RoverPose_yaw(&pose)) / vec.size();
    }
    std::string out_string;
    out_string += std::to_string(x) + "," + std::to_string(y) + "," +
                  std::to_string(yaw) + ",";
    for (auto &tup : vec) {
      char buf[100];
      auto &rssi = std::get<0>(tup);
      RSSIData_bssid(&rssi, buf, sizeof(buf));
      out_string += std::string(buf) + ",";
      out_string += std::to_string(RSSIData_rssi(&rssi)) + ",";
    }
    out_string += "\n";
    out_csv << out_string << std::flush;
    std::cout << "Wrote to csv" << std::endl;

    if (!FLAGS_bokeh_server_address.empty()) {
      try {
      http_client cli(U(FLAGS_bokeh_server_address));
      json::value body = json::value::object();
      body[U("x")] = json::value((int)x);
      body[U("y")] = json::value((int)y);
      body[U("yaw")] = json::value((int)yaw);
      json::value features = json::value::object();
      for (auto &tup : vec) {
        char buf[100];
        auto &rssi = std::get<0>(tup);
        RSSIData_bssid(&rssi, buf, sizeof(buf));
        features[U(buf)] = RSSIData_rssi(&rssi);
      }
      body[U("features")] = features;

      http_response response =
          cli.request(methods::PUT, U("/vector"), body.serialize(),
                      U("application/json"))
              .get();
      if (response.status_code() == status_codes::OK &&
          response.headers().content_type() == U("applicaton/json")) {
        std::cout << "Successfully send vector to bokeh server" << std::endl;
      } else {
        std::cout << "Failed to send vector to bokeh server!" << std::endl;
      }
    } catch (std::exception e) {}
    }
  };

  RoverController rover_ctrl;
  RSSIVectorConstructor ctor;
  ctor.registerCallback(vector_cb);
  ctor.start();
  UART_RECEIVER_Initialize(fd, &ctor, &rover_ctrl);
  UART_TRANSMITTER_Initialize(fd);
  WEBSERVER_MODEL_Initialize();
  SENSORS_MODEL_Initialize();
  RSSI_MODEL_Initialize();
  PID_MODEL_Initialize();
  MOTOR_MODEL_Initialize();
  ASTAR_MODEL_Initialize();

  std::vector<const char *> fake_flags = {"kek", "-c",
                                          FLAGS_server_config.c_str()};

  std::unique_ptr<cppcms::service> srv;
  try {
    srv.reset(new cppcms::service((int)fake_flags.size(),
                                  (char **)fake_flags.data()));
    srv->applications_pool().mount(
        cppcms::applications_factory<webserver_view>());
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  std::thread uart_receiver(uart_receiver_thread_run);
  std::thread uart_transmitter(uart_transmitter_thread_run);
  std::thread webserver_model(webserver_model_thread_run);
  std::thread sensors_model(sensors_model_thread_run);
  std::thread rssi_model(rssi_model_thread_run);
  std::thread pid_model(pid_model_thread_run);
  std::thread motor_model(motor_model_thread_run);
  std::thread errorcheck_model(errorcheck_model_thread_run);
  std::thread warning_model(warning_model_thread_run);
  std::thread encoders_model(encoders_model_thread_run);
  std::thread pose_model(pose_model_thread_run);
  std::thread astar_model(astar_model_thread_run);
  srv->run();
  uart_receiver.join();
  uart_transmitter.join();
  webserver_model.join();
  sensors_model.join();
  rssi_model.join();
  pid_model.join();
  motor_model.join();
  errorcheck_model.join();
  warning_model.join();
  encoders_model.join();
  pose_model.join();
  astar_model.join();
}
