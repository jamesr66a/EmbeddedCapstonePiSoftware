#ifndef _ONLINE_ASTAR_H_
#define _ONLINE_ASTAR_H_

#include "node.h"
#include "TwoDSparseNodeMatrix.h"

//#include <gtest/gtest.h>
#include <functional>
#include <memory>
#include <queue>

namespace online_astar {

#if 0
namespace {
int32_t manhattan_distance(int32_t x1, int32_t x2, int32_t y1, int32_t y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}
}
#endif

class NodePointComparator {
public:
  NodePointComparator(uint32_t x, uint32_t y, uint32_t start_x,
                      uint32_t start_y)
      : x(x), y(y), start_x(start_x), start_y(start_y) {}
  bool operator()(const std::shared_ptr<Node> &lhs,
                  const std::shared_ptr<Node> &rhs) {
    //auto lhs_value = manhattan_distance(lhs->x, start_x, lhs->y, start_y);
    //auto rhs_value = manhattan_distance(rhs->x, start_x, rhs->y, start_y);
    return (lhs->euclidean_distance(x, y)) >
           (rhs->euclidean_distance(x, y));
  }

private:
  uint32_t x, y, start_x, start_y;
};

class OnlineAstar {
public:
  OnlineAstar(uint32_t target_x, uint32_t target_y, uint32_t start_x,
              uint32_t start_y)
      : target_x(target_x), target_y(target_y), start_x(start_x),
        start_y(start_y),
        queue(NodePointComparator(target_x, target_y, start_x, start_y)) {
    bool goal = start_x == target_x && start_y == target_y;
    // Assume start node is never blocked
    queue.push(std::make_shared<Node>(start_x, start_y, goal, false));
  }

  std::shared_ptr<Node> selectSuccessor();
  void reportSuccess(const std::shared_ptr<Node> &in,
                     const std::tuple<int32_t, int32_t> &x_limits,
                     const std::tuple<int32_t, int32_t> &y_limits);

private:
  int32_t target_x, target_y, start_x, start_y;
  std::priority_queue<std::shared_ptr<Node>,
                      std::vector<std::shared_ptr<Node> >,
                      NodePointComparator> queue;
  TwoDSparseNodeMatrix matrix;
  bool initial_step_called = false;

//  FRIEND_TEST(OnlineAStarTest, TestSelectSuccessor);
};

} // namespace online_astar

#endif /* _ONLINE_ASTAR_H_ */
