#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <array>

#include "matrix.hpp"

TEST_CASE("constructor", "[matrix]") {
  matrix<int> m{2, 3};
  for (auto n : m) {
    REQUIRE(n == 0);
  }
}

TEST_CASE("operator[]", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  matrix<int> m{rows, columns};
  for (auto r = 0; r < rows; ++r) {
    for (auto c = 0; c < columns; ++c) {
      REQUIRE(m[{r, c}] == 0);
    }
  }
}

TEST_CASE("for each row", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  matrix<int> m{rows, columns};
  auto values = std::array<int, rows * columns>{1, 2, 3, 4, 5, 6};
  auto index = 0;
  for (auto& n : m) {
    n = values[index++];
  }
  index = 0;
  m.for_each_row([&index, &values](const auto& view) {
    for (auto n : view) {
      REQUIRE(n == values[index++]);
    }
  });
}

TEST_CASE("for each column", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  matrix<int> m{rows, columns};
  auto values = std::array<int, rows * columns>{1, 2, 3, 4, 5, 6};
  auto index = 0;
  for (auto& n : m) {
    n = values[index++];
  }
  auto expected_values = std::array<int, rows * columns>{1, 4, 2, 5, 3, 6};
  index = 0;
  m.for_each_column([&index, &expected_values](const auto& view) {
    for (auto n : view) {
      REQUIRE(n == expected_values[index++]);
    }
  });
}