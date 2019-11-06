#pragma once

#include <iomanip>
#include <ostream>
#include <vector>

#include "matrix.hpp"

struct tile {
  int color = 0;
  int number = 0;
};

inline bool operator==(const tile& lhs, const tile& rhs) noexcept {
  return lhs.color == rhs.color && lhs.number == rhs.number;
}

inline bool operator!=(const tile& lhs, const tile& rhs) noexcept {
  return !(lhs == rhs);
}

inline tile random_tile(int max_number) noexcept {
  return tile{std::rand() % (max_number - 1) + 1,
              std::rand() % (max_number - 1) + 1};
}

struct position {
  position(int r, int c) noexcept : row{r}, column{c} {}

  int row = 0;
  int column = 0;
};

inline bool operator==(const position& lhs, const position& rhs) noexcept {
  return lhs.row == rhs.row && lhs.column == rhs.column;
}

inline bool operator!=(const position& lhs, const position& rhs) noexcept {
  return !(lhs == rhs);
}

using board = matrix<tile>;

inline board init_board(size_t row_count, size_t column_count) {
  board board(row_count, column_count);
  for (size_t r = row_count - 4; r < row_count; ++r) {
    for (auto& tile : board.view_of_row(r)) {
      tile = random_tile(4);
    }
  }
  return board;
}

template <typename V>
inline void match_same(bool for_row, size_t index, V&& view,
                       std::vector<position>& positions) {
  auto it = std::forward<V>(view).begin();
  auto last = 0;
  auto next = 0;
  while (it != std::forward<V>(view).end()) {
    auto current = it + 1;
    next = last + 1;
    while (current != std::forward<V>(view).end() &&
           current->color == it->color) {
      ++current;
      ++next;
    }
    if (next - last >= 3 && *it != tile{}) {
      if (for_row) {
        positions.emplace_back(index, last);
      } else {
        positions.emplace_back(last, index);
      }
    }
    it = current;
    last = next;
  }
}

inline std::vector<position> find_match_color(board& b) {
  std::vector<position> positions;
  b.for_each_row([&positions](size_t r, const auto& row) {
    match_same(true, r, row, positions);
  });
  b.for_each_column([&positions](size_t c, const auto& column) {
    match_same(false, c, column, positions);
  });
  return positions;
}

std::ostream& operator<<(std::ostream& os, board& board) {
  auto index = 0;
  board.for_each_row([&os, &index](size_t, const auto& row) {
    os << std::setw(2) << index++ << " [ ";
    auto first = true;
    for (auto& t : row) {
      if (!first) {
        os << "| ";
      } else {
        first = false;
      }
      if (t != tile{}) {
        os << "C " << t.color << " N " << t.number << " ";
      } else {
        os << "        ";
      }
    }
    os << "]\n";
  });
  return os;
}
