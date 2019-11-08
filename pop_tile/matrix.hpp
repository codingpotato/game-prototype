#pragma once

#include <vector>

template <typename T>
struct matrix {
  struct position {
    position(int r, int c) noexcept : row{r}, column{c} {}
    position(size_t r, size_t c) noexcept
        : row{static_cast<int>(r)}, column{static_cast<int>(c)} {}

    int row = 0;
    int column = 0;
  };

  struct row_iterator {
    row_iterator(matrix* m, size_t i) noexcept : matrix_{m}, index_{i} {}

    row_iterator& operator++() noexcept {
      ++index_;
      return *this;
    }
    row_iterator operator+(int n) const noexcept {
      return row_iterator{matrix_, index_ + n};
    }
    T& operator*() noexcept { return (*matrix_)[index_]; }
    T* operator->() noexcept { return &(*matrix_)[index_]; }

    friend bool operator==(const row_iterator& lhs,
                           const row_iterator& rhs) noexcept {
      return lhs.matrix_ == rhs.matrix_ && lhs.index_ == rhs.index_;
    }
    friend bool operator!=(const row_iterator& lhs,
                           const row_iterator& rhs) noexcept {
      return lhs.matrix_ != rhs.matrix_ || lhs.index_ != rhs.index_;
    }

   private:
    matrix* matrix_;
    size_t index_;
  };

  struct column_iterator {
    column_iterator(matrix* m, size_t i) noexcept : matrix_{m}, index_{i} {}

    column_iterator& operator++() noexcept {
      index_ += matrix_->columns();
      return *this;
    }
    column_iterator operator+(int n) const noexcept {
      return column_iterator{matrix_, index_ + n * matrix_->columns()};
    }
    T& operator*() noexcept { return (*matrix_)[index_]; }
    T* operator->() noexcept { return &(*matrix_)[index_]; }

    friend bool operator==(const column_iterator& lhs,
                           const column_iterator& rhs) noexcept {
      return lhs.matrix_ == rhs.matrix_ && lhs.index_ == rhs.index_;
    }
    friend bool operator!=(const column_iterator& lhs,
                           const column_iterator& rhs) noexcept {
      return lhs.matrix_ != rhs.matrix_ || lhs.index_ != rhs.index_;
    }

   private:
    matrix* matrix_;
    size_t index_;
  };

  struct row_view {
    row_view(matrix* m, size_t r) : matrix_{m}, row_{r} {}

    size_t size() const noexcept { return matrix_->columns(); }

    row_iterator begin() noexcept {
      return row_iterator{matrix_, row_ * matrix_->columns()};
    }
    row_iterator end() noexcept {
      return row_iterator{matrix_, (row_ + 1) * matrix_->columns()};
    }

    template <typename F>
    void for_each(F&& f) noexcept {
      for (size_t c = 0; c < matrix_->columns(); ++c) {
        std::forward<F>(f)(c, (*matrix_)[{row_, c}]);
      }
    }

   private:
    matrix* matrix_;
    size_t row_;
  };

  struct column_view {
    column_view(matrix* m, size_t c) : matrix_{m}, column_{c} {}

    size_t size() const noexcept { return matrix_->rows(); }

    column_iterator begin() noexcept {
      return column_iterator{matrix_, column_};
    }
    column_iterator end() noexcept {
      return column_iterator{matrix_,
                             matrix_->rows() * matrix_->columns() + column_};
    }

    template <typename F>
    void for_each(F&& f) noexcept {
      for (size_t r = 0; r < matrix_->rows(); ++r) {
        std::forward<F>(f)(r, (*matrix_)[{r, column_}]);
      }
    }

   private:
    matrix* matrix_;
    size_t column_;
  };

  matrix(size_t r, size_t c) noexcept
      : rows_{r}, columns_{c}, elements_(rows_ * columns_, T{}) {}

  matrix(size_t r, size_t c, std::initializer_list<T> elements) noexcept
      : rows_{r}, columns_{c}, elements_{elements} {
    assert(elements_.size() == rows_ * columns_);
  }

  size_t rows() const noexcept { return rows_; }
  size_t columns() const noexcept { return columns_; }

  auto begin() const noexcept { return elements_.begin(); }
  auto end() const noexcept { return elements_.end(); }

  T& operator[](position pos) noexcept {
    return elements_[pos.row * columns_ + pos.column];
  }

  T& operator[](size_t index) noexcept { return elements_[index]; }

  template <typename F>
  void for_each_row(F&& f) noexcept {
    for (size_t r = 0; r < rows_; ++r) {
      std::forward<F>(f)(r, row_view{this, r});
    }
  }

  template <typename F>
  void for_each_column(F&& f) noexcept {
    for (size_t c = 0; c < columns_; ++c) {
      std::forward<F>(f)(c, column_view{this, c});
    }
  }

  auto view_of_row(size_t r) noexcept { return row_view{this, r}; }
  auto view_of_column(size_t c) noexcept { return column_view{this, c}; }

 private:
  size_t rows_;
  size_t columns_;
  std::vector<T> elements_;
};