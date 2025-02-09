#pragma once

#include <array>
#include <vector>

namespace color_land {

enum class neighber_type { no_diagonal, all };

struct position {
  int row = 0;
  int column = 0;

  constexpr position() noexcept {}
  constexpr position(int r, int c) noexcept : row{r}, column{c} {}
  constexpr position(size_t r, size_t c) noexcept
      : row{static_cast<int>(r)}, column{static_cast<int>(c)} {}

  friend bool operator==(const position& lhs, const position& rhs) noexcept {
    return lhs.row == rhs.row && lhs.column == rhs.column;
  }
};

using positions = std::vector<position>;

template <typename T>
struct matrix {
  struct iterator : std::iterator<std::forward_iterator_tag, T, int, T*, T&> {
    constexpr iterator(matrix<T>& matrix, size_t index) noexcept
        : matrix_{matrix}, index_{index} {}

    constexpr position pos() const noexcept {
      return position{index_ / matrix_.columns(), index_ % matrix_.columns()};
    }

    iterator& operator++() noexcept {
      ++index_;
      return *this;
    }

    T& operator*() const noexcept { return matrix_[index_]; }
    T* operator->() const noexcept { return &matrix_[index_]; }

    friend bool operator==(const iterator& lhs, const iterator& rhs) noexcept {
      return &lhs.matrix_ == &rhs.matrix_ && lhs.index_ == rhs.index_;
    }
    friend bool operator!=(const iterator& lhs, const iterator& rhs) noexcept {
      return !(lhs == rhs);
    }

   private:
    matrix<T>& matrix_;
    size_t index_;
  };

  struct neighber_view {
    struct iterator : std::iterator<std::forward_iterator_tag, T, int, T*, T&> {
      iterator(const neighber_view& nv, size_t index) noexcept
          : nv_{nv}, index_{index} {
        check_valid();
      }

      constexpr position pos() const noexcept {
        return position{nv_.pos_.row + nv_.direction(index_).row,
                        nv_.pos_.column + nv_.direction(index_).column};
      }

      iterator& operator++() noexcept {
        ++index_;
        check_valid();
        return *this;
      }

      T& operator*() const noexcept { return nv_.matrix_[pos()]; }
      T* operator->() const noexcept { return &nv_.matrix_[pos()]; }

      friend bool operator==(const iterator& lhs,
                             const iterator& rhs) noexcept {
        return &lhs.nv_ == &rhs.nv_ && lhs.index_ == rhs.index_;
      }
      friend bool operator!=(const iterator& lhs,
                             const iterator& rhs) noexcept {
        return !(lhs == rhs);
      }

     private:
      void check_valid() noexcept {
        while (index_ < nv_.directions_size() && !nv_.matrix_.is_valid(pos())) {
          ++index_;
        }
      }

      const neighber_view& nv_;
      size_t index_;
    };

    neighber_view(matrix<T>& matrix, position pos, neighber_type t) noexcept
        : matrix_{matrix}, pos_{pos}, type_{t} {}

    size_t directions_size() const noexcept {
      return type_ == neighber_type::no_diagonal ? directions_.size()
                                                 : directions_diagonal_.size();
    }

    position direction(size_t index) const noexcept {
      return type_ == neighber_type::no_diagonal ? directions_[index]
                                                 : directions_diagonal_[index];
    }

    iterator begin() const noexcept { return iterator{*this, 0}; }
    iterator end() const noexcept { return iterator{*this, directions_size()}; }

   private:
    matrix<T>& matrix_;
    position pos_;
    neighber_type type_;

    static constexpr std::array directions_{position{0, -1}, position{0, 1},
                                            position{-1, 0}, position{1, 0}};
    static constexpr std::array directions_diagonal_{
        position{0, -1},  position{0, 1},  position{-1, 0}, position{1, 0},
        position{-1, -1}, position{1, -1}, position{-1, 1}, position{1, 1}};
  };

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

  bool is_valid(position pos) const noexcept {
    return pos.row >= 0 && static_cast<size_t>(pos.row) < rows_ &&
           pos.column >= 0 && static_cast<size_t>(pos.column) < columns_;
  }

  T& operator[](position pos) noexcept { return elements_[index_of(pos)]; }
  const T& operator[](position pos) const noexcept {
    return elements_[index_of(pos)];
  }

  T& operator[](size_t index) noexcept { return elements_[index]; }
  const T& operator[](size_t index) const noexcept { return elements_[index]; }

  iterator begin() noexcept { return iterator{*this, 0}; }
  iterator end() noexcept { return iterator(*this, rows_ * columns_); }

  neighber_view neighber_view_of(position pos, neighber_type t) noexcept {
    return neighber_view{*this, pos, t};
  }

  template <typename F>
  void for_each(F f) const noexcept {
    for (size_t row = 0; row < rows_; ++row) {
      for (size_t column = 0; column < columns_; ++column) {
        f(position{row, column}, elements_[index_of(row, column)]);
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

template <typename Iterator, typename F>
inline void for_each(Iterator first, Iterator last, F f) noexcept {
  for (auto it = first; it != last; ++it) {
    f(it.pos(), *it);
  }
}

template <typename Iterator>
inline positions all_positions(Iterator first, Iterator last,
                               typename Iterator::value_type v) noexcept {
  positions ps;
  for (auto it = first; it != last; ++it) {
    if (*it == v) {
      ps.push_back(it.pos());
    }
  }
  return ps;
}

template <typename Iterator, typename F>
inline positions all_positions_if(Iterator first, Iterator last, F f) noexcept {
  positions ps;
  for (auto it = first; it != last; ++it) {
    if (f(it.pos(), *it)) {
      ps.push_back(it.pos());
    }
  }
  return ps;
}

}  // namespace color_land