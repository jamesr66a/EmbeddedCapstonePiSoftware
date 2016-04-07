#include <memory>
#include <stdint.h>
#include <unordered_map>

#include "TwoDSparseNodeMatrix.h"

namespace online_astar {

std::shared_ptr<Node> TwoDSparseNodeMatrix::query(int32_t x, int32_t y) {
  auto x_itr = map.find(x);
  if (x_itr == map.end()) {
    return nullptr;
  }

  auto y_itr = x_itr->second.find(y);
  if (y_itr == x_itr->second.end()) {
    return nullptr;
  }

  return y_itr->second;
}

bool TwoDSparseNodeMatrix::insert(const std::shared_ptr<Node>& n) {
  auto ptr = query(n->x, n->y);
  if (ptr.get() != nullptr) {
    return false;
  }

  map[n->x][n->y] = n; 
  return true;
}

} // namespace online_astar
