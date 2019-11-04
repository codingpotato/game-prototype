#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Board.hpp"

TEST_CASE("init", "[Board]") {
  Matrix<int> m(1, 2);
  m.at(0, 0) = 1;
  for (size_t i = 0; i < 1; ++i) {
    for (size_t j = 0; j < 2; ++j) {
      REQUIRE(m.at(i, j) == 0);
    }
  }
}