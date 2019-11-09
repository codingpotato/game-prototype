#pragma once

#include <vector>

struct position {
  position(int r, int c) noexcept : row{r}, column{c} {}

  friend bool operator==(const position& lhs, const position& rhs) noexcept {
    return lhs.row == rhs.row && lhs.column == rhs.column;
  }
  friend bool operator!=(const position& lhs, const position& rhs) noexcept {
    return !(lhs == rhs);
  }
  friend bool operator<(const position& lhs, const position& rhs) noexcept {
    return lhs.row < rhs.row || (lhs.row == rhs.row && lhs.column < rhs.column);
  }

  int row = 0;
  int column = 0;
};

template <typename T>
struct matrix {
  struct base_iterator {
    base_iterator(matrix* m, int i) noexcept : matrix_{m}, index_{i} {}

    T& operator*() noexcept { return (*matrix_)[index_]; }
    T* operator->() noexcept { return &(*matrix_)[index_]; }

    friend bool operator==(const base_iterator& lhs,
                           const base_iterator& rhs) noexcept {
      return lhs.matrix_ == rhs.matrix_ && lhs.index_ == rhs.index_;
    }
    friend bool operator!=(const base_iterator& lhs,
                           const base_iterator& rhs) noexcept {
      return lhs.matrix_ != rhs.matrix_ || lhs.index_ != rhs.index_;
    }

   protected:
    matrix* matrix_;
    int index_;
  };

  struct row_iterator : base_iterator {
    using base_iterator::base_iterator;
    using base_iterator::index_;
    using base_iterator::matrix_;

    row_iterator& operator++() noexcept {
      ++index_;
      return *this;
    }
    row_iterator operator+(int n) const noexcept {
      return row_iterator{matrix_, index_ + n};
    }
  };

  struct row_riterator : base_iterator {
    using base_iterator::base_iterator;
    using base_iterator::index_;
    using base_iterator::matrix_;

    row_riterator& operator++() noexcept {
      --index_;
      return *this;
    }
    row_iterator operator+(int n) const noexcept {
      return row_riterator{matrix_, index_ - n};
    }
  };

  struct column_iterator : base_iterator {
    using base_iterator::base_iterator;
    using base_iterator::index_;
    using base_iterator::matrix_;

    column_iterator& operator++() noexcept {
      index_ += matrix_->columns();
      return *this;
    }
    column_iterator operator+(int n) const noexcept {
      return column_iterator{matrix_, index_ + n * matrix_->columns()};
    }
  };

  struct column_riterator : base_iterator {
    using base_iterator::base_iterator;
    using base_iterator::index_;
    using base_iterator::matrix_;

    column_riterator& operator++() noexcept {
      index_ -= matrix_->columns();
      return *this;
    }
    column_riterator operator+(int n) const noexcept {
      return column_riterator{matrix_, index_ - n * matrix_->columns()};
    }
  };

  struct row_view {
    row_view(matrix* m, int r) : matrix_{m}, row_{r} {}

    int size() const noexcept { return matrix_->columns(); }

    row_iterator begin() noexcept {
      return row_iterator{matrix_, row_ * matrix_->columns()};
    }
    row_iterator end() noexcept {
      return row_iterator{matrix_, (row_ + 1) * matrix_->columns()};
    }
    row_riterator rbegin() noexcept {
      return row_riterator{matrix_, (row_ + 1) * matrix_->columns() - 1};
    }
    row_riterator rend() noexcept {
      return row_riterator{matrix_, row_ * matrix_->columns() - 1};
    }

    template <typename F>
    void for_each(F&& f) noexcept {
      for (int c = 0; c < matrix_->columns(); ++c) {
        std::forward<F>(f)(c, (*matrix_)[{row_, c}]);
      }
    }

   private:
    matrix* matrix_;
    int row_;
  };

  struct column_view {
    column_view(matrix* m, int c) : matrix_{m}, column_{c} {}

    int size() const noexcept { return matrix_->rows(); }

    column_iterator begin() noexcept {
      return column_iterator{matrix_, column_};
    }
    column_iterator end() noexcept {
      return column_iterator{matrix_,
                             matrix_->rows() * matrix_->columns() + column_};
    }
    column_riterator rbegin() noexcept {
      return column_riterator{
          matrix_, (matrix_->rows() - 1) * matrix_->columns() + column_};
    }
    column_riterator rend() noexcept {
      return column_riterator{matrix_, column_ - matrix_->columns()};
    }

    template <typename F>
    void for_each(F&& f) noexcept {
      for (int r = 0; r < matrix_->rows(); ++r) {
        std::forward<F>(f)(r, (*matrix_)[{r, column_}]);
      }
    }

   private:
    matrix* matrix_;
    int column_;
  };

  matrix(int r, int c) noexcept
      : rows_{r}, columns_{c}, elements_(rows_ * columns_, T{}) {}

  matrix(int r, int c, std::initializer_list<T> elements) noexcept
      : rows_{r}, columns_{c}, elements_{elements} {
    assert(elements_.size() == rows_ * columns_);
  }

  int rows() const noexcept { return rows_; }
  int columns() const noexcept { return columns_; }

  bool empty() const noexcept {
    return std::find_if(elements_.begin(), elements_.end(),
                        [](const T& t) { return t != T{}; }) == elements_.end();
  }

  auto begin() const noexcept { return elements_.begin(); }
  auto end() const noexcept { return elements_.end(); }

  T& operator[](position pos) noexcept {
    return elements_[pos.row * columns_ + pos.column];
  }

  T& operator[](size_t index) noexcept { return elements_[index]; }

  template <typename F>
  void for_each_row(F&& f) noexcept {
    for (int r = 0; r < rows_; ++r) {
      std::forward<F>(f)(r, row_view{this, r});
    }
  }

  template <typename F>
  void for_each_column(F&& f) noexcept {
    for (int c = 0; c < columns_; ++c) {
      std::forward<F>(f)(c, column_view{this, c});
    }
  }

  auto view_of_row(int r) noexcept { return row_view{this, r}; }
  auto view_of_column(int c) noexcept { return column_view{this, c}; }

 private:
  int rows_;
  int columns_;
  std::vector<T> elements_;
};