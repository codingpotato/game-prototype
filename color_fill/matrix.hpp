#pragma once

#include <array>
#include <vector>

struct position {
  position(int r, int c) noexcept : row{r}, column{c} {}
  position(size_t r, size_t c) noexcept
      : row{static_cast<int>(r)}, column{static_cast<int>(c)} {}

  int row = 0;
  int column = 0;
};

template <typename T>
struct matrix {
  matrix(size_t rows, size_t columns) noexcept
      : rows_{rows}, columns_{columns}, elements_(rows * columns, T{}) {}

  template <size_t ROWS, size_t COLUMNS>
  matrix(std::array<std::array<T, COLUMNS>, ROWS> elements) noexcept
      : rows_{ROWS}, columns_{COLUMNS} {
    for (auto& row : elements) {
      for (auto& element : row) {
        elements_.push_back(element);
      }
    }
  }

  constexpr size_t rows() const noexcept { return rows_; }
  constexpr size_t columns() const noexcept { return columns_; }

  T& operator[](position pos) noexcept { return elements_[index_of(pos)]; }
  const T& operator[](position pos) const noexcept {
    return elements_[index_of(pos)];
  }

  template <typename F>
  void for_each(F&& f) const noexcept {
    for (size_t row = 0; row < rows_; ++row) {
      for (size_t column = 0; column < columns_; ++column) {
        std::forward<F>(f)(position{row, column},
                           elements_[index_of(row, column)]);
      }
    }
  }

 private:
  size_t index_of(position pos) const noexcept {
    return index_of(pos.row, pos.column);
  }
  size_t index_of(int row, int column) const noexcept {
    return row * columns_ + column;
  }

  size_t rows_;
  size_t columns_;
  std::vector<T> elements_;
};