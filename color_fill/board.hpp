#pragma once

#include "matrix.hpp"

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

struct tile {
  size_t color;
};

bool operator==(const tile& t1, const tile& t2) noexcept {
  return t1.color == t2.color;
}

inline std::ostream& operator<<(std::ostream& os, const tile& t) {
  os << t.color;
  return os;
}

using board = matrix<tile>;

template <typename T>
inline positions empty_neighbers(const matrix<T>& m, position pos) noexcept {
  positions ps;
  std::array<position, 4> moves{position{-1, 0}, position{1, 0},
                                position{0, -1}, position{0, 1}};
  for (auto& move : moves) {
    auto row = pos.row + move.row;
    auto column = pos.column + move.column;
    if (m[position{row, column}] == T{}) {
      ps.emplace_back(row, column);
    }
  }
  return ps;
}

template <typename T>
inline std::pair<position, position> random_empty_neighber_pair(
    const matrix<T>& m) noexcept {
  auto ps = m.all_positions_if([&m](position pos, const T&) {
    return !empty_neighbers(m, pos).empty();
  });
  auto pos = ps[std::rand() % ps.size()];
  auto neighbers = empty_neighbers(m, pos);
  return {pos, neighbers[std::rand() % neighbers.size()]};
}

inline bool fill_two_neighber_pairs(board& b, size_t color) noexcept {
  auto [pos, neighber] = random_empty_neighber_pair(b);
  b[pos] = {color};
  b[neighber] = {color};
  return true;
}