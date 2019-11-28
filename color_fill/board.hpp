#pragma once

#include <matrix.hpp>

struct tile {
  size_t color;
};

using board = matrix<tile>;

template <typename T>
inline std::pair<position, position> find_empty_neighber_tiles(
    const matrix<T>& m) noexcept {
  while (true) {
    position pos{std::rand() % b.rows(), std::rand() % b.columns()};
    if (m[pos] == T{}) {
    }
  }
}

inline bool fill_two_neighber_tiles(board& b, size_t color) noexcept {}