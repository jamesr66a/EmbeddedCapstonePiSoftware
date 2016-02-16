#include <iostream>
#include <thread>

#include "uart_receiver.h"

int main(int argc, char **argv) {
  UART_RECEIVER_Initialize();

  std::thread uart_receiver(uart_receiver_thread_run); 
  uart_receiver.join();
}
