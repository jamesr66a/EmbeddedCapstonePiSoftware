#ifndef _RSSI_MODEL_PUBLIC_H_
#define _RSSI_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

#include <vector>

int rssi_aggregate_debug_info_count();
int rssi_debug_info_rate_per_minute();

void sendToRSSIModelQueue(DebugInfo *info);
std::vector<DebugInfo> rssi_aggregate_info_vector();
 
#endif /* _RSSI_MODEL_PUBLIC_H_ */
