#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "matrix.hpp"

TEST_CASE("constructor", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  std::array<std::array<int, columns>, rows> elements = {
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