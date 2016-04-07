#include "online_astar.h"

#include <gtest/gtest.h>

namespace online_astar {

const uint8_t board[8][10] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                               { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
                               { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
                               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

uint32_t solution[8][10] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
                             { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

uint32_t result[8][10] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
                           { 1, 1, 1, 1, 1, 1, 1, 1, 2, 0 },
                           { 1, 1, 1, 1, 1, 1, 1, 1, 2, 0 },
                           { 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
                           { 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
                           { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 } };

TEST(OnlineAStarTest, TestSelectSuccessor) {
  auto oas = OnlineAstar(0, 0, 7, 0);
  auto node = oas.selectSuccessor();
  ASSERT_EQ(node->x, 7);
  ASSERT_EQ(node->y, 0);
  ASSERT_EQ(node->goal, false);
  ASSERT_EQ(node->blocked, false);

  for (;;) {
    if (board[node->x][node->y]) {
      node->blocked = true;
    } else {
      solution[node->x][node->y] = 2;
    }

    oas.reportSuccess(node, std::make_tuple(0, 7), std::make_tuple(0, 9));
    //std::cout << "(" << node->x << "," << node->y << ")"
    //          << (board[node->x][node->y] ? "b" : "") << std::endl;

    node = oas.selectSuccessor();
    if (node->x == 0 && node->y == 0) {
      std::cout << "Reached goal state\n";
      break;
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 10; j++) {
      //ASSERT_EQ(solution[i][j], result[i][j]);
      std::cout << solution[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

} // namespace online_astar
