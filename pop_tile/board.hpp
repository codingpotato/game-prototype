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

using board = matrix<tile>;
using positions = std::vector<board::position>;

inline board init_board(size_t row_count, size_t column_count) noexcept {
  board b(row_count, column_count);
  for (size_t r = row_count - 4; r < row_count; ++r) {
    b.view_of_row(r).for_each([](size_t, tile& t) { t = random_tile(4); });
  }
  return b;
}

template <typename V, typename F>
inline void match_same(size_t index, V&& view, positions& ps, F&& f) noexcept {
  decltype(f(tile{})) last_property;
  size_t last_index = 0;
  view.for_each([&index, &view, &ps, &f, &last_property, &last_index](size_t i,
                                                                      tile& t) {
    if (i == 0) {
      last_property = f(t);
      last_index = 0;
    } else {
      auto add_position = [](size_t index, size_t last_index, positions& ps) {
        if constexpr (std::is_same_v<std::decay_t<V>, board::row_view>) {
          ps.emplace_back(index, last_index);
        } else {
          ps.emplace_back(last_index, index);
        }
      };
      if (f(t) != last_property) {
        if (last_property != 0 && i - last_index >= 3) {
          add_position(index, last_index, ps);
        }
        last_property = f(t);
        last_index = i;
      } else if (i == view.size() - 1 && last_property != 0 &&
                 i - last_index + 1 >= 3) {
        add_position(index, last_index, ps);
      }
    }
  });
}

template <typename F>
inline positions match_same(board& b, F&& f) noexcept {
  positions ps;
  b.for_each_row(
      [&ps, &f](size_t r, auto&& row) { match_same(r, row, ps, f); });
  b.for_each_column(
      [&ps, &f](size_t c, auto&& column) { match_same(c, column, ps, f); });
  return ps;
}

inline std::pair<positions, positions> match_same(board& b) noexcept {
  return {match_same(b, [](const tile& t) { return t.color; }),
          match_same(b, [](const tile& t) { return t.number; })};
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
