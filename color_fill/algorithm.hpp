#pragma once

#include <iostream>

#include "matrix.hpp"

using board = matrix<int>;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, matrix<T>& m) {
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
  return os;
}
