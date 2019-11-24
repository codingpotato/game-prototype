#pragma once

#include <array>
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

using board = matrix<int>;

template <typename T>
inline bool is_in_matrix(const matrix<T>& m, int row, int column) {
  return row >= 0 && static_cast<size_t>(row) < m.rows() && column >= 0 &&
         static_cast<size_t>(column) < m.columns();
}

template <typename F>
inline int enclosure_count(const board& b, int row, int column, F&& f) {
  assert(is_in_matrix(b, row, column));

  constexpr size_t direction = 8;
  const std::array<position, direction> moves{
      position{-1, 0}, position{-1, -1}, position{0, -1}, position{1, -1},
      position{1, 0},  position{1, 1},   position{0, 1},  position{-1, 1}};
  int count = 0;
  for (auto& m : moves) {
    int r = row + m.row;
    int c = column + m.column;
    if (is_in_matrix(b, r, c) &&
        std::forward<F>(f)(b, position(row, column), position(r, c))) {
      ++count;
    }
  }
  return count;
}

inline board generate_enclosure_board(const board& b) {
  board enclosure{b.rows(), b.columns()};
  for (size_t row = 0; row < b.rows(); ++row) {
    for (size_t column = 0; column < b.columns(); ++column) {
      enclosure[position(row, column)] = enclosure_count(
          b, row, column, [](const board& b, position pos, position neighber) {
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
    int r = pos.row + m.row;
    int c = pos.column + m.column;
    if (is_in_matrix(status_m, r, c) &&
        status_m[position(r, c)] != status::shown) {
      status_m[position(r, c)] = status::hidden;
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
        position pos(row, column);
        if (!visited[pos] &&
            enclosure_count(
                b, row, column,
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
