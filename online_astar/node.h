#ifndef _NODE_H_
#define _NODE_H_

#include <gtest/gtest.h>
#include <stdint.h>
#include <tuple>
#include <vector>

namespace online_astar {

struct Node {
  Node(int32_t x, int32_t y, bool goal = false, bool blocked = false)
      : x(x), y(y), goal(goal), blocked(blocked) {}

  Node(const Node &rhs)
      : x(rhs.x), y(rhs.y), goal(rhs.goal), blocked(rhs.blocked) {}

  std::vector<std::tuple<int32_t, int32_t> >
  successors(const std::tuple<int32_t, int32_t> &x_bounds,
             const std::tuple<int32_t, int32_t> &y_bounds) const;

  int32_t x;
  int32_t y;

  bool goal;
  bool blocked;

  float euclidean_distance(int32_t other_x, int32_t other_y);
}; // struct Node

} // namespace online_astar

#endif /* _NODE_H_ */
