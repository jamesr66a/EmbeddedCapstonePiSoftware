#ifndef _ERRORCHECK_MODEL_PUBLIC_H_
#define _ERRORCHECK_MODEL_PUBLIC_H_ 

#include "generated/DebugInfo.pbo.h"

#include <vector>

int errorcheck_aggregate_debug_info_count();
float errorcheck_debug_info_rate_per_minute();

void sendToErrorCheckModelQueue(DebugInfo *info);
std::vector<DebugInfo> errorcheck_aggregate_info_vector();

#endif /* _ERRORCHECK_MODEL_PUBLIC_H_ */
