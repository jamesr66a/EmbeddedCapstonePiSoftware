#include "motor_model.h"

#include "debug.h"

MOTOR_MODEL_DATA motor_modelData;

// Public functions

void sendToMOTORModelQueue(DebugInfo *info) {
  motor_modelData.input_queue.enqueue(*info);
}

int motor_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(motor_modelData.data_mutex);
  return motor_modelData.total_num;
}

int motor_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(motor_modelData.data_mutex);
  return motor_modelData.rate_per_min;
}

std::vector<DebugInfo> motor_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(motor_modelData.data_mutex);
  return motor_modelData.data_vec;
}

// Internal functions

void MOTOR_MODEL_Initialize() { motor_modelData.state = MOTOR_MODEL_INIT; }

void MOTOR_MODEL_Tasks() {
  switch (motor_modelData.state) {
  case MOTOR_MODEL_INIT: {
    motor_modelData.state = MOTOR_MODEL_RECEIVE;
    motor_modelData.beginning = std::chrono::system_clock::now();
    motor_modelData.total_num = 0;
    motor_modelData.rate_per_min = 0.0;
  } break;
  case MOTOR_MODEL_RECEIVE: {
    auto receivedData = motor_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(motor_modelData.data_mutex);
      if (motor_modelData.data_vec.size() == 50) {
        motor_modelData.data_vec.erase(motor_modelData.data_vec.begin());
      }
      motor_modelData.data_vec.push_back(receivedData);
      motor_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         motor_modelData.beginning).count();
      motor_modelData.rate_per_min =
          60 * ((float)motor_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void motor_model_thread_run() {
  while (true) {
    MOTOR_MODEL_Tasks();
  }
}
