#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "matrix.hpp"

TEST_CASE("constructor", "[matrix]") {
  matrix<int> m{2, 3};
  for (auto n : m) {
    REQUIRE(n == 0);
  }
}

TEST_CASE("operator()", "[matrix]") {
  constexpr size_t rows = 2;
  constexpr size_t columns = 3;
  matrix<int> m{rows, columns};
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      REQUIRE(m(r, c) == 0);
    }
  }
}