#include "node.h"

#include <cmath>

namespace online_astar {

float Node::euclidean_distance(int32_t other_x, int32_t other_y) {
  using std::sqrt;
  int x_norm = x - other_x;
  int y_norm = y - other_y;
  return sqrt(x_norm * x_norm + y_norm * y_norm);
}

} // namespace online_astar
