#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "debug.h"
#include "serial.h"
#include "uart_receiver.h"
#include "uart_transmitter.h"

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

  std::thread uart_receiver(uart_receiver_thread_run);
  std::thread uart_transmitter(uart_transmitter_thread_run);
  uart_receiver.join();
  uart_transmitter.join();
}
