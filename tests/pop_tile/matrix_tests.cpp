#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <array>

#include "matrix.hpp"

TEST_CASE("constructor", "[matrix]") {
  matrix<int> m{2, 3};
  REQUIRE(m.rows() == 2);
  REQUIRE(m.columns() == 3);
  for (auto n : m) {
    REQUIRE(n == 0);
  }
}

TEST_CASE("subscription", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  matrix<int> m{rows, columns};
  m[{1, 1}] = 1;
  for (auto r = 0; r < rows; ++r) {
    for (auto c = 0; c < columns; ++c) {
      if (r == 1 && c == 1) {
        REQUIRE(m[{r, c}] == 1);
      } else {
        REQUIRE(m[{r, c}] == 0);
      }
    }
  }
}

TEST_CASE("for each row", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  std::initializer_list<int> values{1, 2, 3, 4, 5, 6};
  matrix<int> m{rows, columns, values};
  auto expected_values = std::array<int, rows * columns>{1, 2, 3, 4, 5, 6};
  auto index = 0;
  m.for_each_row([&index, &expected_values](size_t, auto&& row) {
    row.for_each([&index, &expected_values](size_t, int n) {
      REQUIRE(n == expected_values[index++]);
    });
  });
}

TEST_CASE("for in row", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  std::initializer_list<int> values{1, 2, 3, 4, 5, 6};
  matrix<int> m{rows, columns, values};
  auto expected_values = std::array<int, rows * columns>{1, 2, 3, 4, 5, 6};
  auto index = 0;
  for (size_t r = 0; r < rows; ++r) {
    for (auto n : m.view_of_row(r)) {
      REQUIRE(n == expected_values[index++]);
    }
  }
}

TEST_CASE("for each column", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  std::initializer_list<int> values{1, 2, 3, 4, 5, 6};
  matrix<int> m{rows, columns, values};
  auto expected_values = std::array<int, rows * columns>{1, 4, 2, 5, 3, 6};
  auto index = 0;
  m.for_each_column([&index, &expected_values](size_t, auto&& column) {
    column.for_each([&index, &expected_values](size_t, int n) {
      REQUIRE(n == expected_values[index++]);
    });
  });
}

TEST_CASE("for in column", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  std::initializer_list<int> values{1, 2, 3, 4, 5, 6};
  matrix<int> m{rows, columns, values};
  auto expected_values = std::array<int, rows * columns>{1, 4, 2, 5, 3, 6};
  auto index = 0;
  for (size_t c = 0; c < columns; ++c) {
    for (auto n : m.view_of_column(c)) {
      REQUIRE(n == expected_values[index++]);
    }
  }
}