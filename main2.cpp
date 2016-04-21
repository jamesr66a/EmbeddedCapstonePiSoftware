#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <fcntl.h>
#include <glog/logging.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "debug.h"
#include "encoders_model.h"
#include "errorcheck_model.h"
#include "generated/RoverPose.pbo.h"
#include "generated/RSSIData.pbo.h"
#include "motor_model.h"
#include "pid_model.h"
#include "pose_model.h"
#include "rssi_model.h"
#include "RoverController.h"
#include "RSSIVectorConstructor.h"
#include "serial.h"
#include "sensors_model.h"
#include "uart_receiver.h"
#include "uart_transmitter.h"
#include "warning_model.h"
#include "webserver_model.h"
#include "webserver_view.h"

RoverController rover_controller;

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "USAGE: main uart_filename csv_filename\n";
    return -1;
  }

  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = "./logs";
  LOG(INFO) << "test";

  // Initialize receive device file
  int fd = open(argv[1], O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    perror("error opening");
    errorCheck();
  }
  set_interface_attribs(fd, B115200, 0);
  set_blocking(fd, 1);

  std::ofstream out_csv;
  out_csv.open(argv[2], std::ios_base::app);

  auto vector_cb =
      [&out_csv](const std::vector<std::tuple<RSSIData, RoverPose> > &vec) {
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
      };

  RSSIVectorConstructor ctor;
  ctor.registerCallback(vector_cb);
  ctor.start();
  UART_RECEIVER_Initialize(fd, &ctor, &rover_controller);
  UART_TRANSMITTER_Initialize(fd);
  WEBSERVER_MODEL_Initialize();
  SENSORS_MODEL_Initialize();
  RSSI_MODEL_Initialize();
  PID_MODEL_Initialize();
  MOTOR_MODEL_Initialize();

  std::unique_ptr<cppcms::service> srv;
  try {
    srv.reset(new cppcms::service(argc, argv));
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
  rover_controller.start();
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
}
