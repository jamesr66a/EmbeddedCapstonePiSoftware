#ifndef _NODE_H_
#define _NODE_H_

#include <gtest/gtest.h>
#include <stdint.h>

namespace online_astar {

struct Node {
  Node(int32_t x, int32_t y, bool goal = false, bool blocked = false)
      : x(x), y(y), goal(goal), blocked(blocked) {}

  int32_t x;
  int32_t y;

  bool goal;
  bool blocked;

  float euclidean_distance(int32_t other_x, int32_t other_y);
}; // struct Node

} // namespace online_astar

#endif /* _NODE_H_ */
