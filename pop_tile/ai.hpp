#pragma once

#include <limits>

#include "board.hpp"

inline position predict(board& b) noexcept {
  position pos;
  auto max_count = std::numeric_limits<int>::min();
  b.for_each([&b, &pos, &max_count](int r, int c, tile& t) {
    if (t != tile{}) {
      auto predict_board = b;
      auto count = remove_neighber(predict_board, r, c);
      while (auto n = match_same(predict_board)) {
        count += n;
      }
      if (is_game_over(predict_board)) {
        count = 0;
      }
      if (count > max_count) {
        pos = {r, c};
        max_count = count;
      }
    }
  });
  return pos;
}