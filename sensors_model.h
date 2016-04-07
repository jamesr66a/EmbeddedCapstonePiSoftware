#ifndef _SENSORS_MODEL_H_
#define _SENSORS_MODEL_H_

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "sensors_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  SENSORS_MODEL_INIT,
  SENSORS_MODEL_RECEIVE,
} SENSORS_MODEL_STATES;

typedef struct {
  SENSORS_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} SENSORS_MODEL_DATA;

void SENSORS_MODEL_Initialize();
void SENSORS_MODEL_Tasks();
void sensors_model_thread_run();

#endif /* _SENSORS_MODEL_H_ */
