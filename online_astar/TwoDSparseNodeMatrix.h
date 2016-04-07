#ifndef _TWODSPARSENODEMATRIX_H_
#define _TWODSPARSENODEMATRIX_H_

#include "node.h"

#include <gtest/gtest_prod.h>
#include <memory>
#include <stdint.h>
#include <unordered_map>

namespace online_astar {

class TwoDSparseNodeMatrix {
public:
  TwoDSparseNodeMatrix() {}
  std::shared_ptr<Node> query(int32_t x, int32_t y);
  bool insert(const std::shared_ptr<Node>& n);

private:
  std::unordered_map<int32_t,
                     std::unordered_map<int32_t, std::shared_ptr<Node> > > map;

  FRIEND_TEST(TwoDSparseNodeMatrixTest, TestMatrixConstruction);
  FRIEND_TEST(TwoDSparseNodeMatrixTest, TestInsertionAndFind);
}; // class TwoDSparseNodeMatrix

} // namespace online_astar

#endif /* _TWODSPARSENODEMATRIX_H_ */
