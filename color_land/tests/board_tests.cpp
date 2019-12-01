#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "board.hpp"

TEST_CASE("constructor", "[matrix]") {
  board b{6, 6};
  initialize_board(b, 6);
  for (tile c = 1; c <= 6; ++c) {
    REQUIRE(std::count(b.begin(), b.end(), c) == 2);
  }
}