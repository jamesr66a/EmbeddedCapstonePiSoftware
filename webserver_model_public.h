#ifndef _WEBSERVER_MODEL_PUBLIC_H_
#define _WEBSERVER_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

int aggregate_debug_info_count();
float aggregate_debug_info_rate_per_minute();

void sendToWebserverModelQueue(DebugInfo *info);

#endif /* _WEBSERVER_MODEL_PUBLIC_H_ */
