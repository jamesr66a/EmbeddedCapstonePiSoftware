#ifndef _MOTOR_MODEL_H_
#define _MOTOR_MODEL_H_ 

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "motor_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  MOTOR_MODEL_INIT,
  MOTOR_MODEL_RECEIVE,
} MOTOR_MODEL_STATES;

typedef struct {
  MOTOR_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} MOTOR_MODEL_DATA;

void MOTOR_MODEL_Initialize();
void MOTOR_MODEL_Tasks();
void motor_model_thread_run();

#endif /* _MOTOR_MODEL_H_ */
