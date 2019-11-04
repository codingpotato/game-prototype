#pragma once

template <typename T>
struct matrix {
  enum dimension { row, column };

  template <dimension D>
  struct iterator {
   private:
    matrix& matrix_;
  };

  template <dimension D>
  struct view {
    view(const matrix& matrix, size_t index) noexcept
        : matrix_{matrix}, index_{index} {}

   private:
    matrix& matrix_;
    size_t index_;
  };

  matrix(size_t rows, size_t columns) noexcept
      : rows_{rows}, columns_{columns}, elements_(rows * columns, T{}) {}

  size_t rows() const noexcept { return rows_; }
  size_t columns() const noexcept { return columns_; }

  T& operator()(size_t row, size_t column) noexcept {
    return elements_[index_of(row, column)];
  }

  view<row> operator[](size_t row) const noexcept { return {*this, row}; }

  auto begin() noexcept { return elements_.begin(); }
  auto end() noexcept { return elements_.end(); }
  auto cbegin() const noexcept { return elements_.cbegin(); }
  auto cend() const noexcept { return elements_.cend(); }

 private:
  size_t index_of(size_t row, size_t column) const noexcept {
    return row * columns_ + column;
  }

  size_t rows_;
  size_t columns_;
  std::vector<T> elements_;
};