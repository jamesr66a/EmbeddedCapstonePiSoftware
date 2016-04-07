#include "sensors_model.h"

#include "debug.h"

SENSORS_MODEL_DATA sensors_modelData;

// Public functions

void sendToSensorsModelQueue(DebugInfo *info) {
  sensors_modelData.input_queue.enqueue(*info);
}

int sensors_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(sensors_modelData.data_mutex);
  return sensors_modelData.total_num;
}

float sensors_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(sensors_modelData.data_mutex);
  return sensors_modelData.rate_per_min;
}

std::vector<DebugInfo> sensors_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(sensors_modelData.data_mutex);
  return sensors_modelData.data_vec;
}

// Internal functions

void SENSORS_MODEL_Initialize() {
  sensors_modelData.state = SENSORS_MODEL_INIT;
}

void SENSORS_MODEL_Tasks() {
  switch (sensors_modelData.state) {
  case SENSORS_MODEL_INIT: {
    sensors_modelData.state = SENSORS_MODEL_RECEIVE;
    sensors_modelData.beginning = std::chrono::system_clock::now();
    sensors_modelData.total_num = 0;
    sensors_modelData.rate_per_min = 0.0;
  } break;
  case SENSORS_MODEL_RECEIVE: {
    auto receivedData = sensors_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(sensors_modelData.data_mutex);
      if (sensors_modelData.data_vec.size() == 50) {
        sensors_modelData.data_vec.erase(sensors_modelData.data_vec.begin());
      }
      sensors_modelData.data_vec.push_back(receivedData);
      sensors_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         sensors_modelData.beginning).count();
      sensors_modelData.rate_per_min =
          60 * ((float)sensors_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void sensors_model_thread_run() {
  while (true) {
    SENSORS_MODEL_Tasks();
  }
}
