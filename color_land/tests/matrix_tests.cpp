#define CATCH_CONFIG_MAIN
#include <algorithm>
#include <catch2/catch.hpp>

#include "matrix.hpp"

TEST_CASE("constructor", "[matrix]") {
  color_land::matrix<int> m{2, 3};
  for (auto n : m) {
    REQUIRE(n == 0);
  }
}

TEST_CASE("iterator", "[matrix]") {
  color_land::matrix<int> m{2, 3};
  std::vector<color_land::position> expected_positions{{0, 0}, {0, 1}, {0, 2},
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
  color_land::matrix<int> m{elements};
  for (size_t row = 0; row < rows; ++row) {
    for (size_t column = 0; column < columns; ++column) {
      REQUIRE(m[color_land::position{row, column}] == elements[row][column]);
    }
  }
}

TEST_CASE("neighber view", "[matrix]") {
  color_land::matrix<int> m{2, 3};
  auto count = 0;
  for (auto n :
       m.neighber_view_of({0, 0}, color_land::neighber_type::no_diagonal)) {
    REQUIRE(n == 0);
    ++count;
  }
  REQUIRE(count == 2);

  auto nv = m.neighber_view_of({0, 0}, color_land::neighber_type::all);
  REQUIRE(std::count(nv.begin(), nv.end(), 0) == 3);
}

TEST_CASE("all positions of matrix", "[matrix]") {
  color_land::matrix<int> m{2, 3};
  auto ps = color_land::all_positions(std::begin(m), std::end(m), 0);
  REQUIRE(ps == color_land::positions{
                    color_land::position{0, 0}, color_land::position{0, 1},
                    color_land::position{0, 2}, color_land::position{1, 0},
                    color_land::position{1, 1}, color_land::position{1, 2}});
}

TEST_CASE("all positions of neighber view", "[matrix]") {
  color_land::matrix<int> m{2, 3};
  auto neighber_view = m.neighber_view_of(
      color_land::position{0, 0}, color_land::neighber_type::no_diagonal);
  auto ps = color_land::all_positions(std::begin(neighber_view),
                                      std::end(neighber_view), 0);
  REQUIRE(ps == color_land::positions{color_land::position{0, 1},
                                      color_land::position{1, 0}});
}