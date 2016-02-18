#include "debug.h"
#include "webserver_model.h"

#include <iostream>

WEBSERVER_MODEL_DATA webserver_modelData;

// Public functions
void sendToWebserverModelQueue(DebugInfo *info) {
  webserver_modelData.input_queue.enqueue(*info);
}

int aggregate_debug_info_count() {
  std::lock_guard<std::mutex>(webserver_modelData.data_mutex);
  return webserver_modelData.total_num;
}

float aggregate_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex>(webserver_modelData.data_mutex);
  return webserver_modelData.rate_per_min;
}

std::vector<DebugInfo> aggregate_info_vector() {
  std::lock_guard<std::mutex>(webserver_modelData.data_mutex);
  return webserver_modelData.data_vec;
}

// Internal functions

void WEBSERVER_MODEL_Initialize() {
  webserver_modelData.state = WEBSERVER_MODEL_INIT;
}

void WEBSERVER_MODEL_Tasks() {
  switch (webserver_modelData.state) {
  case WEBSERVER_MODEL_INIT: {
    webserver_modelData.state = WEBSERVER_MODEL_RECEIVE;
    webserver_modelData.beginning = std::chrono::system_clock::now();
    webserver_modelData.total_num = 0;
    webserver_modelData.rate_per_min = 0.0;
  } break;
  case WEBSERVER_MODEL_RECEIVE: {
    auto receivedData = webserver_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex>(webserver_modelData.data_mutex);
      if (webserver_modelData.data_vec.size() == 50) {
        webserver_modelData.data_vec.erase(
            webserver_modelData.data_vec.begin());
      }
      webserver_modelData.data_vec.push_back(receivedData);
      webserver_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         webserver_modelData.beginning).count();
      webserver_modelData.rate_per_min =
          60 * ((float)webserver_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void webserver_model_thread_run() {
  while (true) {
    WEBSERVER_MODEL_Tasks();
  }
}
