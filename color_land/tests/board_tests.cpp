#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "board.hpp"

inline void show_board(const color_land::board& b) noexcept {
  std::cout << "  ";
  for (size_t column = 0; column < b.columns(); ++column) {
    std::cout << " " << column << "  ";
  }
  std::cout << "\n";
  for (size_t row = 0; row < b.rows(); ++row) {
    std::cout << row << " [ ";
    for (size_t column = 0; column < b.columns(); ++column) {
      if (column > 0) {
        std::cout << "| ";
      }
      std::cout << b[color_land::position{row, column}].raw_value << " ";
    }
    std::cout << "]\n";
  }
  std::cout << "\n";
}

TEST_CASE("constructor", "[board]") {
  color_land::board b{6, 6};
  fill_seeds_in_board(b, 6);
  for (auto value = 1; value <= 6; ++value) {
    REQUIRE(std::count(b.begin(), b.end(), color_land::color{value}) == 2);
  }
}

TEST_CASE("all", "[board]") {
  std::srand(std::time(nullptr));
  color_land::board b{7, 7};
  fill_board(b, 8);
  show_board(b);
}