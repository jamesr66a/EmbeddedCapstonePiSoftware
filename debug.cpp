#include "debug.h"

#include <iostream>
#include <stdlib.h>

void errorCheck() {
  std::cerr << "error" << std::endl;
  exit(-1);
}
