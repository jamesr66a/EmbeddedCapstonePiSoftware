#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "debug.h"
#include "serial.h"
#include "uart_receiver.h"
#include "uart_transmitter.h"
#include "webserver_model.h"
#include "webserver_view.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "USAGE: main uart_filename\n";
    return -1;
  }

  // Initialize receive device file
  int fd = open(argv[1], O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    perror("error opening");
    errorCheck();
  }
  set_interface_attribs(fd, B115200, 0);
  set_blocking(fd, 1);

  UART_RECEIVER_Initialize(fd);
  UART_TRANSMITTER_Initialize(fd);
  WEBSERVER_MODEL_Initialize();

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
  srv->run();
  uart_receiver.join();
  uart_transmitter.join();
  webserver_model.join();
}
