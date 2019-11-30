#pragma once

#include <algorithm>
#include <iostream>
#include <optional>

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

using tile = size_t;
using board = matrix<tile>;

template <typename T>
inline std::optional<std::pair<position, position>> random_empty_neighber_pair(
    matrix<T>& m) noexcept {
  auto ps = m.all_positions_if([&m](position pos, const T& t) {
    if (t != T{}) {
      return false;
    }
    auto nv = m.neighber_view_of(pos, neighber_type::no_diagonal);
    return std::count(nv.begin(), nv.end(), T{}) > 0;
  });
  if (ps.empty()) {
    return {};
  }

  auto pos = ps[std::rand() % ps.size()];
  auto nv = m.neighber_view_of(pos, neighber_type::no_diagonal);
  positions neighbers;
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    if (*it == T{}) {
      neighbers.push_back(it.pos());
    }
  }
  return std::make_optional<std::pair<position, position>>(
      pos, neighbers[std::rand() % neighbers.size()]);
}

inline bool fill_two_neighber_pairs(board& b, size_t color) noexcept {
  if (auto neighber_pair = random_empty_neighber_pair(b)) {
    auto [pos, neighber] = *neighber_pair;
    b[pos] = color;
    b[neighber] = color;
    return true;
  }
  return false;
}

inline void initialize_board(board& b, size_t colors) noexcept {
  for (size_t c = 1; c <= colors; ++c) {
    fill_two_neighber_pairs(b, c);
  }
}