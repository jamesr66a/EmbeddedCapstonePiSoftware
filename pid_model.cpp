#include "pid_model.h"

#include "debug.h"

PID_MODEL_DATA pid_modelData;

// Public functions

void sendToPIDModelQueue(DebugInfo *info) {
  pid_modelData.input_queue.enqueue(*info);
}

int pid_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(pid_modelData.data_mutex);
  return pid_modelData.total_num;
}

int pid_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(pid_modelData.data_mutex);
  return pid_modelData.rate_per_min;
}

std::vector<DebugInfo> pid_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(pid_modelData.data_mutex);
  return pid_modelData.data_vec;
}

// Internal functions

void PID_MODEL_Initialize() {
  pid_modelData.state = PID_MODEL_INIT;
}

void PID_MODEL_Tasks() {
  switch (pid_modelData.state) {
    case PID_MODEL_INIT: {
      pid_modelData.state = PID_MODEL_RECEIVE;
      pid_modelData.beginning = std::chrono::system_clock::now();
      pid_modelData.total_num = 0;
      pid_modelData.rate_per_min = 0.0;
    } break;
  case PID_MODEL_RECEIVE: {                                                 
    auto receivedData = pid_modelData.input_queue.dequeue();                
                                                                                
    {
      std::lock_guard<std::mutex> guard(pid_modelData.data_mutex);
      if (pid_modelData.data_vec.size() == 50) {
        pid_modelData.data_vec.erase(pid_modelData.data_vec.begin());   
      }
      pid_modelData.data_vec.push_back(receivedData);
      pid_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         pid_modelData.beginning).count();
      pid_modelData.rate_per_min =
          60 * ((float)pid_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void pid_model_thread_run() {
  while (true) {
    PID_MODEL_Tasks();
  }
}

