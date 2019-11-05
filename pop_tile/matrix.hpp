#pragma once

template <typename T>
struct matrix {
  enum class dimension { row, column };

  template <dimension D>
  struct iterator {
    iterator(matrix& m, size_t i) noexcept : matrix_{m}, index_{i} {}

    friend bool operator==(const iterator& lhs, const iterator& rhs) noexcept {
      return &lhs.matrix_ == &rhs.matrix_ && lhs.index_ == rhs.index_;
    }

    friend bool operator!=(const iterator& lhs, const iterator& rhs) noexcept {
      return !(lhs == rhs);
    }

    T& operator*() noexcept { return matrix_[index_]; }

    iterator& operator++() noexcept {
      if constexpr (D == dimension::row) {
        ++index_;
        return *this;
      } else {
        index_ += matrix_.columns();
        return *this;
      }
    }

   private:
    matrix& matrix_;
    size_t index_;
  };

  template <dimension D>
  struct view {
    view(matrix& m, size_t i) noexcept : matrix_{m}, index_{i} {}

    iterator<D> begin() const noexcept {
      if constexpr (D == dimension::row) {
        return iterator<D>{matrix_, index_ * matrix_.columns()};
      } else {
        return iterator<D>{matrix_, index_};
      }
    }
    iterator<D> end() const noexcept {
      if constexpr (D == dimension::row) {
        return iterator<D>{matrix_, (index_ + 1) * matrix_.columns()};
      } else {
        return iterator<D>{matrix_,
                           matrix_.rows() * matrix_.columns() + index_};
      }
    }

   private:
    matrix& matrix_;
    size_t index_;
  };

  struct position {
    int row;
    int column;
  };

  matrix(size_t rows, size_t columns) noexcept
      : rows_{rows}, columns_{columns}, elements_(rows * columns, T{}) {}

  size_t rows() const noexcept { return rows_; }
  size_t columns() const noexcept { return columns_; }

  const T& operator[](position pos) const noexcept {
    return elements_[index_of(pos.row, pos.column)];
  }
  T& operator[](position pos) noexcept {
    return elements_[index_of(pos.row, pos.column)];
  }

  const T& operator[](size_t index) const noexcept { return elements_[index]; }
  T& operator[](size_t index) noexcept { return elements_[index]; }

  template <typename F>
  void for_each_row(F&& f) noexcept {
    for (size_t r = 0; r < rows_; ++r) {
      std::forward<F>(f)(view<dimension::row>{*this, r});
    }
  }

  template <typename F>
  void for_each_column(F&& f) noexcept {
    for (size_t c = 0; c < columns_; ++c) {
      std::forward<F>(f)(view<dimension::column>{*this, c});
    }
  }

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