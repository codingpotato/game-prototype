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
  board b(row_count, column_count);
  for (size_t r = row_count - 4; r < row_count; ++r) {
    b.view_of_row(r).for_each([](size_t, tile& t) { t = random_tile(4); });
  }
  return b;
}

template <typename V, typename F>
inline void match_same(size_t index, V&& view, std::vector<position>& positions,
                       F&& f) {
  decltype(f(tile{})) last_property;
  auto last_index = 0;
  view.for_each([&index, &view, &positions, &f, &last_property, &last_index](
                    size_t i, tile& t) {
    if (i == 0) {
      last_property = f(t);
      last_index = 0;
    } else {
      auto add_position = [&positions](size_t index, size_t last_index) {
        if constexpr (std::is_same_v<std::decay_t<V>, board::row_view>) {
          positions.emplace_back(index, last_index);
        } else {
          positions.emplace_back(last_index, index);
        }
      };

      if (f(t) != last_property) {
        if (last_property != 0 && i - last_index >= 3) {
          add_position(index, last_index);
        }
        last_property = f(t);
        last_index = i;
      } else if (i == view.size() - 1) {
        if (last_property != 0 && i - last_index + 1 >= 3) {
          add_position(index, last_index);
        }
      }
    }
  });
}

inline std::vector<position> find_match_color(board& b) {
  std::vector<position> positions;
  b.for_each_row([&positions](size_t r, auto&& row) {
    match_same(r, row, positions, [](const tile& t) { return t.color; });
  });
  b.for_each_column([&positions](size_t c, auto&& column) {
    match_same(c, column, positions, [](const tile& t) { return t.color; });
  });
  return positions;
}

std::ostream& operator<<(std::ostream& os, board& b) {
  b.for_each_row([&os](size_t r, auto&& row) {
    os << std::setw(2) << r << " [ ";
    row.for_each([&os](size_t c, tile& t) {
      if (c > 0) {
        os << "| ";
      }
      if (t != tile{}) {
        os << "C " << t.color << " N " << t.number << " ";
      } else {
        os << "        ";
      }
    });
    os << "]\n";
  });
  return os;
}
