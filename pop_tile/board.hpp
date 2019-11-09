#pragma once

#include <iomanip>
#include <ostream>
#include <set>
#include <vector>

#include "matrix.hpp"

struct tile {
  int color = 0;
  int number = 0;

  friend bool operator==(const tile& lhs, const tile& rhs) noexcept {
    return lhs.color == rhs.color && lhs.number == rhs.number;
  }
  friend bool operator!=(const tile& lhs, const tile& rhs) noexcept {
    return !(lhs == rhs);
  }
};

inline tile random_tile(int max_number) noexcept {
  return tile{std::rand() % (max_number - 1) + 1,
              std::rand() % (max_number - 1) + 1};
}

using board = matrix<tile>;
using positions = std::vector<position>;

inline board init_board(size_t row_count, size_t column_count) noexcept {
  board b(row_count, column_count);
  for (size_t r = row_count - 4; r < row_count; ++r) {
    b.view_of_row(r).for_each([](size_t, tile& t) { t = random_tile(4); });
  }
  return b;
}

template <typename V, typename F>
inline void match_same_3(size_t index, V&& view, positions& ps,
                         F&& f) noexcept {
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
inline void match_neighber(board& b, position pos, F&& f,
                           std::set<position>& result,
                           matrix<int>& visited) noexcept {
  if (visited[pos] == 0) {
    result.insert(pos);
    visited[pos] = 1;
    const static std::vector<position> ds{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (auto& d : ds) {
      auto r = pos.row + d.row;
      auto c = pos.column + d.column;
      if (r >= 0 && static_cast<size_t>(r) < b.rows() && c >= 0 &&
          static_cast<size_t>(c) < b.columns() && f(b[pos]) == f(b[{r, c}])) {
        match_neighber(b, {r, c}, f, result, visited);
      }
    }
  }
}

template <typename F>
inline positions match_neighber(board& b, const positions& ps, F&& f) noexcept {
  std::set<position> result;
  matrix<int> visited(b.rows(), b.columns());
  for (auto& pos : ps) {
    match_neighber(b, pos, f, result, visited);
  }
  return positions{result.begin(), result.end()};
}

template <typename F>
inline positions match_same(board& b, F&& f) noexcept {
  positions ps;
  b.for_each_row(
      [&ps, &f](size_t r, auto&& row) { match_same_3(r, row, ps, f); });
  b.for_each_column(
      [&ps, &f](size_t c, auto&& column) { match_same_3(c, column, ps, f); });
  return match_neighber(b, ps, f);
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
