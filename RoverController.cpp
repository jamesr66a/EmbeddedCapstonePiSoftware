#include "RoverController.h"
#include "uart_transmitter_public.h"

bool RoverController::sendToQueue(const UART_RECEIVER_VARIANT_WIRE &var) {
  queue.enqueue(var);
  return true;
}

void RoverController::start() {
  std::lock_guard<std::mutex> guard(mut);
  thread.reset(new std::thread(&RoverController::worker_fn, this));
}

namespace {
constexpr int32_t grid_size = 30;
constexpr int32_t
    x_vec[] = { 0, 0, -grid_size, -grid_size, 0, 0, grid_size, grid_size,
                0, 0, -grid_size, -grid_size, 0, 0, grid_size, grid_size,
                0 };
constexpr int32_t
    y_vec[] = { 0, grid_size,  grid_size,  0, 0, grid_size,  grid_size,  0,
                0, -grid_size, -grid_size, 0, 0, -grid_size, -grid_size, 0,
                0 };

size_t pair_idx = 0;
}

RoverController::RoverController() {}

void RoverController::process() {
  queue.dequeue();

  pair_idx++;
  pair_idx %= (sizeof(x_vec) / sizeof(x_vec[0]));
  UART_TRANSMITTER_VARIANT var;
  var.type = CONTROL_CMD;
  RoverPose_init(&var.data.controlCmd);
  RoverPose_set_xPosition(&var.data.controlCmd, x_vec[pair_idx]);
  RoverPose_set_yPosition(&var.data.controlCmd, y_vec[pair_idx]);
  RoverPose_set_yaw(&var.data.controlCmd, 0);
  RoverPose_to_bytes(&var.data.controlCmd, (char *)&var.data.controlCmd, 0);
  sendToUartQueue(&var);
}

void RoverController::worker_fn() {
  while (true) {
    process();
  }
}
