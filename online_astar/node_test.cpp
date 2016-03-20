#include "node.h"

using namespace online_astar;

TEST(NodeTest, TestNodeConstructor) {
  Node a(3, 4);
  EXPECT_EQ(3, a.x);
  EXPECT_EQ(4, a.y);
  EXPECT_EQ(false, a.goal);
  EXPECT_EQ(false, a.blocked);

  Node b(4, 3, true, true);
  EXPECT_EQ(4, b.x);
  EXPECT_EQ(3, b.y);
  EXPECT_EQ(true, b.goal);
  EXPECT_EQ(true, b.blocked); // TODO: blocked goal state may be a fatal
                              // condition. Investigate
}

TEST(NodeTest, EuclideanDistanceTest) {
  Node a(1234, 4321);
  EXPECT_FLOAT_EQ(4204.572392f, a.euclidean_distance(999, 123));
}
