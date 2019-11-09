#pragma once

#include <iomanip>
#include <ostream>
#include <set>
#include <vector>

#include "matrix.hpp"

constexpr int max_random = 8;

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
    b.view_of_row(r).for_each(
        [](size_t, tile& t) { t = random_tile(max_random); });
  }
  return b;
}

template <typename V, typename F>
inline void match_same_3(int index, V&& view, positions& ps, F&& f) noexcept {
  decltype(f(tile{})) last_property;
  auto last_index = 0;
  view.for_each(
      [&index, &view, &ps, &f, &last_property, &last_index](int i, tile& t) {
        if (i == 0) {
          last_property = f(t);
          last_index = 0;
        } else {
          auto add_position = [](int index, int last_index, positions& ps) {
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
      if (r >= 0 && r < b.rows() && c >= 0 && c < b.columns() &&
          f(b[pos]) == f(b[{r, c}])) {
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

inline void fall_down(board& b) noexcept {
  b.for_each_column([](int, auto&& column) {
    auto it_blank = std::find(column.rbegin(), column.rend(), tile{});
    if (it_blank != column.rend()) {
      auto it = it_blank + 1;
      while (it != column.rend()) {
        if (*it != tile{}) {
          std::swap(*it_blank, *it);
          it_blank = std::find(it_blank, column.rend(), tile{});
        }
        ++it;
      }
    }
  });
}

inline int match_same(board& b) noexcept {
  auto ps_color = match_same(b, [](const tile& t) { return t.color; });
  auto ps_number = match_same(b, [](const tile& t) { return t.number; });
  auto remove_same = [](board& b, auto& ps) {
    for (auto& pos : ps) {
      b[pos] = tile{};
    }
  };
  remove_same(b, ps_color);
  remove_same(b, ps_number);
  fall_down(b);
  return ps_color.size() + ps_number.size();
}

inline int remove_neighber(board& b, int r, int c) noexcept {
  if (b[{r, c}] == tile{}) return 0;
  auto ps_color =
      match_neighber(b, {{r, c}}, [](const tile& t) { return t.color; });
  auto ps_number =
      match_neighber(b, {{r, c}}, [](const tile& t) { return t.number; });
  auto remove_same = [](board& b, auto& ps) {
    int count = 0;
    for (auto& pos : ps) {
      if (b[pos] != tile{}) {
        b[pos] = tile{};
        ++count;
      }
    }
    return count;
  };
  int count = 0;
  count += remove_same(b, ps_color);
  count += remove_same(b, ps_number);
  fall_down(b);
  return count;
}

inline bool is_game_over(board& b) {
  auto view = b.view_of_row(0);
  return std::find_if(view.begin(), view.end(),
                      [](const tile& t) { return t != tile{}; }) != view.end();
}

inline void generate_new_row(board& b) {
  b.for_each_row([&b](int r, auto&& row) {
    if (r < b.rows() - 1) {
      row.for_each([&b, r](int c, tile& t) { t = b[{r + 1, c}]; });
    }
  });
  b.view_of_row(b.rows() - 1).for_each([](int, tile& t) {
    t = random_tile(max_random);
  });
}

inline std::ostream& operator<<(std::ostream& os, board& b) {
  b.for_each_row([&os](int r, auto&& row) {
    os << std::setw(2) << r << " [ ";
    row.for_each([&os](int c, tile& t) {
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
