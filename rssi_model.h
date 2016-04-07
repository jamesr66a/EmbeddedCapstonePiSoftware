#ifndef _RSSI_MODEL_H_
#define _RSSI_MODEL_H_ 

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "rssi_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  RSSI_MODEL_INIT,
  RSSI_MODEL_RECEIVE,
} RSSI_MODEL_STATES;

typedef struct {
  RSSI_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} RSSI_MODEL_DATA;

void RSSI_MODEL_Initialize();
void RSSI_MODEL_Tasks();
void rssi_model_thread_run();

#endif /* _RSSI_MODEL_H_ */
