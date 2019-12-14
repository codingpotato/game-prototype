#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "board.hpp"

TEST_CASE("constructor", "[board]") {
  color_land::board b{6, 6};
  fill_seeds_in_board(b, 6);
  for (auto value = 1; value <= 6; ++value) {
    REQUIRE(std::count(b.begin(), b.end(), color_land::color{value}) == 2);
  }
}
