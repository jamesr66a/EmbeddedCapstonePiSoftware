#include <cmath>
#include <fstream>

#include "astar_model.h"
#include "uart_transmitter_public.h"

#include "debug.h"

#define LOWER_LIMIT (std::numeric_limits<int32_t>::min())
#define UPPER_LIMIT (std::numeric_limits<int32_t>::max())

ASTAR_MODEL_DATA astar_modelData;

// Public functions

void sendToAStarModelQueue(AStarVariant *info) {
  astar_modelData.input_queue.enqueue(*info);
}

// Internal functions

std::ofstream astar_log("./astar_log.txt");

void ASTAR_MODEL_Initialize() { astar_modelData.state = ASTAR_MODEL_INIT; }

namespace {
void sendCommand(int32_t x, int32_t y) {
  struct UART_TRANSMITTER_VARIANT var;
  var.type = CONTROL_CMD;
  RoverPose_init(&var.data.controlCmd);
  RoverPose_set_xPosition(&var.data.controlCmd, x);
  RoverPose_set_yPosition(&var.data.controlCmd, y);
  RoverPose_set_yaw(&var.data.controlCmd, 0);
  RoverPose_to_bytes(&var.data.controlCmd, (char *)&var.data.controlCmd, 0);
  sendToUartQueue(&var);
}
}

void ASTAR_MODEL_Tasks() {
  switch (astar_modelData.state) {
  case ASTAR_MODEL_INIT: {
    astar_modelData.prev.reset(new online_astar::Node(0, 0, false, false));
    astar_modelData.curr.reset(new online_astar::Node(0, 0, false, false));
    astar_modelData.state = ASTAR_MODEL_RECEIVE;
  } break;
  case ASTAR_MODEL_RECEIVE: {
    auto received_msg = astar_modelData.input_queue.dequeue();
    switch (received_msg.type) {
    case ASTAR_SETPOINT: {
      astar_modelData.target_x =
          RoverPose_xPosition(&received_msg.data.astar_setpoint);
      astar_modelData.target_y =
          RoverPose_yPosition(&received_msg.data.astar_setpoint);

      astar_modelData.oas.reset(new online_astar::OnlineAstar(
          astar_modelData.target_x, astar_modelData.target_y,
          astar_modelData.current_x, astar_modelData.current_y));

      astar_log << "New setpoint: " << astar_modelData.target_x << " "
                << astar_modelData.target_y << std::endl;
      astar_log << "Current position: " << astar_modelData.current_x << " "
                << astar_modelData.current_y << std::endl;
      astar_modelData.curr = astar_modelData.oas->selectSuccessor();

      astar_modelData.astar_running = true;
      if (astar_modelData.curr) {
        sendCommand(astar_modelData.curr->x, astar_modelData.curr->y);
        astar_log << "Successor: " << astar_modelData.curr->x << " "
                  << astar_modelData.curr->y << std::endl;
      } else {
        astar_modelData.astar_running = false;
        astar_log << "No more points to explore" << std::endl;
      }
    } break;
    case NODE_BLOCKED: {
      if (!astar_modelData.curr->blocked && astar_modelData.astar_running) {
        astar_log << "Node blocked!" << std::endl;
        astar_modelData.curr->blocked = true;
      }
    } break;
    case ASTAR_MOVE_COMPLETE: {
      if (astar_modelData.astar_running) {
        if (std::sqrt(
                std::pow(astar_modelData.curr->x - astar_modelData.target_x,
                         2.0) +
                std::pow(astar_modelData.curr->y - astar_modelData.target_y,
                         2.0)) < 7.07) {
          astar_log << "Navigation complete!" << std::endl;
          astar_modelData.astar_running = false;
          break;
        }

        astar_modelData.oas->reportSuccess(
            astar_modelData.curr, std::make_tuple(LOWER_LIMIT, UPPER_LIMIT),
            std::make_tuple(LOWER_LIMIT, UPPER_LIMIT));
        astar_modelData.prev = astar_modelData.curr;
        astar_modelData.curr = astar_modelData.oas->selectSuccessor();

        if (astar_modelData.curr) {
          sendCommand(astar_modelData.curr->x, astar_modelData.curr->y);
          astar_log << "Successor: " << astar_modelData.curr->x << " "
                    << astar_modelData.curr->y << std::endl;
        } else {
          astar_log << "No more points to explore" << std::endl;
          astar_modelData.astar_running = false;
        }
      }
    } break;
    case CURRENT_POSITION_X: {
      astar_modelData.current_x = received_msg.data.position;
#ifdef ASTAR_VERBOSE
      astar_log << "Current position update X: " << astar_modelData.current_x
                << std::endl;
#endif
    } break;
    case CURRENT_POSITION_Y: {
      astar_modelData.current_y = received_msg.data.position;
#ifdef ASTAR_VERBOSE
      astar_log << "Current position update Y: " << astar_modelData.current_y
                << std::endl;
#endif
    } break;
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void astar_model_thread_run() {
  while (true) {
    ASTAR_MODEL_Tasks();
  }
}
