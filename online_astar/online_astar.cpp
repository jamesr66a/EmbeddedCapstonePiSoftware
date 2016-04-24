#include "online_astar.h"

#include <cassert>
#include <limits>

namespace online_astar {

std::shared_ptr<Node> OnlineAstar::selectSuccessor() {
  if (queue.empty()) {
    return nullptr;
  }

  std::shared_ptr<Node> retval;
  do {
    retval = queue.top();
    queue.pop();
  } while (matrix.query(retval->x, retval->y));
  matrix.insert(retval);
  return retval;
}

void OnlineAstar::reportSuccess(const std::shared_ptr<Node> &in,
                                const std::tuple<int32_t, int32_t> &x_limits,
                                const std::tuple<int32_t, int32_t> &y_limits) {
  if (!in->blocked) {
    auto successors = in->successors(x_limits, y_limits);
    for (auto s : successors) {
      auto x = std::get<0>(s), y = std::get<1>(s);
      auto query_ptr = matrix.query(x, y);
      if (query_ptr == nullptr) {
        bool is_goal = (x == target_x && y == target_y);
        auto instance = std::make_shared<Node>(x, y, is_goal, false);
        queue.push(instance);
      } //  if (query_ptr == nullptr)
    }   // for (auto s : successors)
  }     // if (!in->blocked)
}

} // namespace online_astar
