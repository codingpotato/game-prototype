#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "board.hpp"

TEST_CASE("find match color", "[board]") {
  std::initializer_list<tile> tiles{{1, 2}, {1, 2}, {1, 2}};
  board b{1, 3, tiles};
  auto positions = match_same(b, [](const tile& t) { return t.color; });
  REQUIRE(positions.size() == 1);
  REQUIRE(positions[0] == position{0, 0});
}

TEST_CASE("find match color 1", "[board]") {
  std::initializer_list<tile> tiles{{1, 2}, {1, 2}, {1, 2}};
  board b{3, 1, tiles};
  auto positions = match_same(b, [](const tile& t) { return t.color; });
  REQUIRE(positions.size() == 1);
  REQUIRE(positions[0] == position{0, 0});
}