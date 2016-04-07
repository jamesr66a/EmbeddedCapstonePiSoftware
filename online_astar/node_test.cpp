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

TEST(NodeTest, TestSuccessorFn) {
#if 0
  Node x(3, 4);
  auto s =
      x.successors(std::make_tuple(-5, 5000), std::make_tuple(-2000, 2000));
#ifdef T
#error T is already defined
#endif
#define T(x, y) std::make_tuple((x), (y))
  std::tuple<int32_t, int32_t> expected[] = { T(2, 3), T(3, 3), T(4, 3),
                                              T(2, 4), T(3, 4), T(4, 4),
                                              T(2, 5), T(3, 5), T(4, 5) };
  EXPECT_EQ(s.size(), 9);
  for (size_t i = 0; i < s.size(); i++) {
    EXPECT_EQ(expected[i], s[i]);
  }

  // Now test truncation due to constraints
  s = x.successors(std::make_tuple(4, 6), std::make_tuple(0, 4));
  std::tuple<int32_t, int32_t> expected2[] = { T(4, 3), T(4, 4) };

  EXPECT_EQ(s.size(), 2);
  for (size_t i = 0; i < s.size(); i++) {
    EXPECT_EQ(expected2[i], s[i]);
  }
#undef T
#endif
  Node x(3, 4);
  auto s =
      x.successors(std::make_tuple(-5, 5000), std::make_tuple(-2000, 2000));
#ifdef T
#error T is already defined
#endif
#define T(x, y) std::make_tuple((x), (y))
  std::tuple<int32_t, int32_t> expected[] = { T(4, 4), T(2, 4), T(3, 5),
                                              T(3, 3) };
  EXPECT_EQ(s.size(), 4);
  for (size_t i = 0; i < s.size(); i++) {
    EXPECT_EQ(expected[i], s[i]);
  }
#undef T
}
