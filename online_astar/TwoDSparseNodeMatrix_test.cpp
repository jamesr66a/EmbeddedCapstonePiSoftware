#include "TwoDSparseNodeMatrix.h"

#include <gtest/gtest.h>

namespace online_astar {

TEST(TwoDSparseNodeMatrixTest, TestMatrixConstruction) {
  TwoDSparseNodeMatrix matrix;
  EXPECT_EQ(true, matrix.map.empty());
}

TEST(TwoDSparseNodeMatrixTest, TestInsertionAndFind) {
  TwoDSparseNodeMatrix matrix;
  EXPECT_EQ(true, matrix.map.empty());

  // Test successful insertion
  Node test_node(1, 3, false, true);
  EXPECT_EQ(true, matrix.insert(std::make_shared<Node>(test_node)));
  auto found = matrix.query(1, 3);
  EXPECT_NE(nullptr, found.get());
  EXPECT_EQ(found->x, test_node.x);
  EXPECT_EQ(found->y, test_node.y);
  EXPECT_EQ(found->blocked, test_node.blocked);
  EXPECT_EQ(found->goal, test_node.goal);

  // Test unsuccessful insertion (collision)
  Node second_test_node(1, 3, true, false);
  EXPECT_EQ(false, matrix.insert(std::make_shared<Node>(second_test_node)));
  found = matrix.query(1, 3);
  EXPECT_NE(nullptr, found.get());
  EXPECT_EQ(found->x, test_node.x);
  EXPECT_EQ(found->y, test_node.y);
  EXPECT_EQ(found->blocked, test_node.blocked);
  EXPECT_EQ(found->goal, test_node.goal);
}

} // namespace online_astar
