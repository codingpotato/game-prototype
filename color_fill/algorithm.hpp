#pragma once

#include <array>
#include <cassert>
#include <iostream>

#include "matrix.hpp"

enum class status { unknown, shown, hidden };

inline std::ostream& operator<<(std::ostream& os, status s) {
  switch (s) {
    case status::unknown:
      os << " ";
      break;
    case status::shown:
      os << "S";
      break;
    case status::hidden:
      os << "H";
      break;
  }
  return os;
}

template <typename T>
inline bool is_in_matrix(const matrix<T>& m, position pos) {
  return pos.row >= 0 && static_cast<size_t>(pos.row) < m.rows() &&
         pos.column >= 0 && static_cast<size_t>(pos.column) < m.columns();
}

using tile = int;
using board = matrix<tile>;

template <typename F>
inline std::vector<position> filter(const board& b, F&& f) {
  std::vector<position> positions;
  b.for_each([&f, &positions](position pos, const tile& t) {
    if (std::forward<F>(f)(pos, t)) {
      positions.push_back(pos);
    }
  });
  return positions;
}

inline void fill_first_color(board& b, int color) noexcept {
  auto positions = filter(b, [](position, const tile& t) { return t == 0; });
  auto pos = positions[std::rand() % positions.size()];
  b[pos] = color;
}

inline std::vector<tile> neighbers(const board& b, position pos) noexcept {
  constexpr size_t direction = 4;
  static const std::array<position, direction> moves{
      position{-1, 0},
      position{0, -1},
      position{1, 0},
      position{0, 1},
  };
  std::vector<tile> tiles;
  for (auto move : moves) {
    position new_pos{pos.row + move.row, pos.column + move.column};
    if (is_in_matrix(b, new_pos) && b[new_pos] != tile{}) {
      tiles.push_back(b[new_pos]);
    }
  }
  return tiles;
}

inline void fill_board(board& b, int colors) noexcept {
  for (auto i = 1; i <= colors; ++i) {
    fill_first_color(b, i);
  }
  while (true) {
    auto positions = filter(b, [&b](position pos, const tile& t) {
      return t == tile{} && !neighbers(b, pos).empty();
    });
    if (positions.empty()) {
      break;
    }
    auto pos = positions[std::rand() % positions.size()];
    auto ns = neighbers(b, pos);
    b[pos] = ns[std::rand() % ns.size()];
  }
}

template <typename F>
inline int enclosure_count(const board& b, position pos, F&& f) {
  assert(is_in_matrix(b, pos));

  constexpr size_t direction = 8;
  const std::array<position, direction> moves{
      position{-1, 0}, position{-1, -1}, position{0, -1}, position{1, -1},
      position{1, 0},  position{1, 1},   position{0, 1},  position{-1, 1}};
  int count = 0;
  for (auto& m : moves) {
    position new_pos{pos.row + m.row, pos.column + m.column};
    if (is_in_matrix(b, new_pos) && std::forward<F>(f)(b, pos, new_pos)) {
      ++count;
    }
  }
  return count;
}

inline board generate_enclosure_board(const board& b) {
  board enclosure{b.rows(), b.columns()};
  for (size_t row = 0; row < b.rows(); ++row) {
    for (size_t column = 0; column < b.columns(); ++column) {
      position pos{static_cast<int>(row), static_cast<int>(column)};
      enclosure[pos] = enclosure_count(
          b, pos, [](const board& b, position pos, position neighber) {
            return b[pos] == b[neighber];
          });
    }
  }
  return enclosure;
}

inline void set_shown_status(matrix<status>& status_m, position pos) {
  if (status_m[pos] == status::unknown) {
    status_m[pos] = status::shown;
  }
  constexpr size_t direction = 8;
  const std::array<position, direction> moves{
      position{-1, 0}, position{-1, -1}, position{0, -1}, position{1, -1},
      position{1, 0},  position{1, 1},   position{0, 1},  position{-1, 1}};
  for (auto& m : moves) {
    position new_pos{pos.row + m.row, pos.column + m.column};
    if (is_in_matrix(status_m, new_pos) && status_m[new_pos] != status::shown) {
      status_m[new_pos] = status::hidden;
    }
  }
}

inline matrix<status> generate_status_matrix(const board& b,
                                             const board& enclosure,
                                             matrix<int>& visited) {
  matrix<status> status_m(b.rows(), b.columns());
  bool found = true;
  do {
    found = false;
    for (size_t row = 0; row < b.rows(); ++row) {
      for (size_t column = 0; column < b.columns(); ++column) {
        position pos{static_cast<int>(row), static_cast<int>(column)};
        if (!visited[pos] &&
            enclosure_count(
                b, pos,
                [&status_m](const board& b, position pos, position neighber) {
                  return status_m[neighber] == status::unknown ||
                         b[pos] == b[neighber];
                }) == enclosure[pos]) {
          set_shown_status(status_m, pos);
          visited[pos] = 1;
          found = true;
        }
      }
    }
  } while (found == true);
  return status_m;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
  for (size_t r = 0; r < m.rows(); ++r) {
    os << " [ ";
    for (size_t c = 0; c < m.columns(); ++c) {
      if (c > 0) {
        os << "| ";
      }
      os << m[position(r, c)] << " ";
    }
    os << "]\n";
  }
  os << "\n";
  return os;
}
