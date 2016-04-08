#ifndef _ENCODERS_MODEL_H_
#define _ENCODERS_MODEL_H_

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "encoders_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  ENCODERS_MODEL_INIT,
  ENCODERS_MODEL_RECEIVE,
} ENCODERS_MODEL_STATES;

typedef struct {
  ENCODERS_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} ENCODERS_MODEL_DATA;

void ENCODERS_MODEL_Initialize();
void ENCODERS_MODEL_Tasks();
void encoders_model_thread_run();

#endif /* _ENCODERS_MODEL_H_ */
