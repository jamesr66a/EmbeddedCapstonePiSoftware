#ifndef _ROVERCONTROLLER_H_
#define _ROVERCONTROLLER_H_

#include "SafeQueue.h"
#include "uart_receiver_public.h"

#include <thread>

class RoverController {
public:
  RoverController();
  bool sendToQueue(const UART_RECEIVER_VARIANT_WIRE &var);
  void start();
private:
  void process();
  void worker_fn();

  std::unique_ptr<std::thread> thread;
  SafeQueue<struct UART_RECEIVER_VARIANT_WIRE> queue;

  mutable std::mutex mut;
};

#endif /* _ROVERCONTROLLER_H_ */
