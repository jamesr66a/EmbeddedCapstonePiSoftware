#include "errorcheck_model.h"

#include "debug.h"

ERRORCHECK_MODEL_DATA errorcheck_modelData;

// Public functions

void sendToErrorCheckModelQueue(DebugInfo *info) {
  errorcheck_modelData.input_queue.enqueue(*info);
}

int errorcheck_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(errorcheck_modelData.data_mutex);
  return errorcheck_modelData.total_num;
}

float errorcheck_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(errorcheck_modelData.data_mutex);
  return errorcheck_modelData.rate_per_min;
}

std::vector<DebugInfo> errorcheck_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(errorcheck_modelData.data_mutex);
  return errorcheck_modelData.data_vec;
}

// Internal functions

void ERRORCHECK_MODEL_Initialize() {
  errorcheck_modelData.state = ERRORCHECK_MODEL_INIT;
}

void ERRORCHECK_MODEL_Tasks() {
  switch (errorcheck_modelData.state) {
  case ERRORCHECK_MODEL_INIT: {
    errorcheck_modelData.state = ERRORCHECK_MODEL_RECEIVE;
    errorcheck_modelData.beginning = std::chrono::system_clock::now();
    errorcheck_modelData.total_num = 0;
    errorcheck_modelData.rate_per_min = 0.0;
  } break;
  case ERRORCHECK_MODEL_RECEIVE: {
    auto receivedData = errorcheck_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(errorcheck_modelData.data_mutex);
      if (errorcheck_modelData.data_vec.size() == 50) {
        errorcheck_modelData.data_vec.erase(
            errorcheck_modelData.data_vec.begin());
      }
      errorcheck_modelData.data_vec.push_back(receivedData);
      errorcheck_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         errorcheck_modelData.beginning).count();
      errorcheck_modelData.rate_per_min =
          60 * ((float)errorcheck_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void errorcheck_model_thread_run() {
  while (true) {
    ERRORCHECK_MODEL_Tasks();
  }
}
