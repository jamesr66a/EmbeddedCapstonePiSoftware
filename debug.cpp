#include "debug.h"

#include <glog/logging.h>
#include <iostream>
#include <stdlib.h>

void errorCheck() {
  LOG(ERROR) << "error" << std::endl;
  exit(-1);
}
