#pragma once

#include <vector>

struct Position {
  Position(int r, int c) noexcept : row{r}, column{c} {}

  int row = 0;
  int column = 0;
};

template <typename T>
struct matrix {
  matrix(std::vector<std::vector<T>> elements) noexcept
      : rows_{elements.size()}, columns_{elements[0].size()} {
    for (auto& row : elements) {
      for (auto& element : row) {
        elements_.push_back(element);
      }
    }
  }

  constexpr size_t rows() const noexcept { return rows_; }
  constexpr size_t columns() const noexcept { return columns_; }

  T& operator[](Position pos) { return elements_[index_of_position(pos)]; }

 private:
  size_t index_of_position(Position pos) const noexcept {
    return index_of_position(pos.row, pos.column);
  }
  size_t index_of_position(int row, int column) const noexcept {
    return row * columns_ + column;
  }

  size_t rows_;
  size_t columns_;
  std::vector<T> elements_;
};