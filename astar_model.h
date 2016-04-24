#ifndef _ASTAR_MODEL_H_
#define _ASTAR_MODEL_H_

#include "astar_model_public.h"
#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "online_astar/online_astar.h"

#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

typedef enum {
  ASTAR_MODEL_INIT,
  ASTAR_MODEL_RECEIVE,
} ASTAR_MODEL_STATES;

typedef struct {
  ASTAR_MODEL_STATES state;

  SafeQueue<AStarVariant> input_queue;

  std::mutex data_mutex;

  bool astar_running = false;
  
  std::unique_ptr<online_astar::OnlineAstar> oas;

  std::shared_ptr<online_astar::Node> prev, curr;

  int32_t current_x, current_y, target_x, target_y;
} ASTAR_MODEL_DATA;

void ASTAR_MODEL_Initialize();
void ASTAR_MODEL_Tasks();
void astar_model_thread_run();

#endif /* _ASTAR_MODEL_H_ */
