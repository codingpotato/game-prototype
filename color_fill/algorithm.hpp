#pragma once

#include <array>
#include <iostream>

#include "matrix.hpp"

using board = matrix<int>;

inline bool is_in_board(const board& b, int row, int column) {
  return row >= 0 && static_cast<size_t>(row) < b.rows() && column >= 0 &&
         static_cast<size_t>(column) < b.columns();
}

inline int enclosure_count(const board& b, int row, int column) {
  assert(is_in_board(b, row, column));

  constexpr size_t direction = 8;
  const std::array<Position, direction> moves{
      Position{-1, 0}, Position{-1, -1}, Position{0, -1}, Position{1, -1},
      Position{1, 0},  Position{1, 1},   Position{0, 1},  Position{-1, 1}};
  int count = 0;
  for (auto& m : moves) {
    int r = row + m.row;
    int c = column + m.column;
    if (is_in_board(b, r, c) && b[Position(row, column)] == b[Position(r, c)]) {
      ++count;
    }
  }
  return count;
}

inline board generate_enclosure_board(const board& b) {
  board enclosure{b.rows(), b.columns()};
  for (size_t row = 0; row < b.rows(); ++row) {
    for (size_t column = 0; column < b.columns(); ++column) {
      enclosure[Position(row, column)] = enclosure_count(b, row, column);
    }
  }
  return enclosure;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
  for (size_t r = 0; r < m.rows(); ++r) {
    os << " [ ";
    for (size_t c = 0; c < m.columns(); ++c) {
      if (c > 0) {
        os << "| ";
      }
      os << m[Position(r, c)] << " ";
    }
    os << "]\n";
  }
  os << "\n";
  return os;
}
