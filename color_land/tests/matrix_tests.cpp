#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <algorithm>

#include "matrix.hpp"

TEST_CASE("constructor", "[matrix]") {
  matrix<int> m{2, 3};
  for (auto n : m) {
    REQUIRE(n == 0);
  }
}

TEST_CASE("iterator", "[matrix]") {
  matrix<int> m{2, 3};
  std::vector<position> expected_positions{{0, 0}, {0, 1}, {0, 2},
                                           {1, 0}, {1, 1}, {1, 2}};
  auto index = 0;
  for (auto it = m.begin(); it != m.end(); ++it) {
    REQUIRE(it.pos() == expected_positions[index++]);
  }
}

TEST_CASE("subscription", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  constexpr std::array<std::array<int, columns>, rows> elements = {
      std::array<int, columns>{1, 2, 3},
      std::array<int, columns>{4, 5, 6},
  };
  matrix<int> m{elements};
  for (size_t row = 0; row < rows; ++row) {
    for (size_t column = 0; column < columns; ++column) {
      REQUIRE(m[position{row, column}] == elements[row][column]);
    }
  }
}

TEST_CASE("neighber_view", "[matrix]") {
  matrix<int> m{2, 3};
  auto count = 0;
  for (auto n : m.neighber_view_of({0, 0}, neighber_type::no_diagonal)) {
    REQUIRE(n == 0);
    ++count;
  }
  REQUIRE(count == 2);

  auto nv = m.neighber_view_of({0, 0}, neighber_type::diagonal);
  REQUIRE(std::count(nv.begin(), nv.end(), 0) == 3);
}