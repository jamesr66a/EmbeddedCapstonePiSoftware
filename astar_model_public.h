#ifndef _ASTAR_MODEL_PUBLIC_H_
#define _ASTAR_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"
#include "generated/IRSensorData.pbo.h"
#include "generated/RoverPose.pbo.h"

#include <vector>

typedef enum {
  ASTAR_SETPOINT,
  SENSOR_INFO,
  ASTAR_MOVE_COMPLETE,
  CURRENT_POSITION_X,
  CURRENT_POSITION_Y
} AStarVariant_Type;

struct AStarVariant {
  AStarVariant_Type type;
  union {
    RoverPose astar_setpoint;
    IRSensorData ir_data;
    int32_t position;
  } data; 
};

void sendToAStarModelQueue(AStarVariant *info);
// TODO: pair accessor fn

#endif /* _ASTAR_MODEL_PUBLIC_H_ */
