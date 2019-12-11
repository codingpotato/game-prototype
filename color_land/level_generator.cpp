#include <ctime>
#include <iostream>

#include "board.hpp"

inline char char_of_color(color_land::color c) noexcept {
  return 'A' + c.raw_value - 1;
}

inline void show_board(const color_land::board& b) noexcept {
  for (size_t row = 0; row < b.rows(); ++row) {
    std::cout << " [ ";
    for (size_t column = 0; column < b.columns(); ++column) {
      if (column > 0) {
        std::cout << "| ";
      }
      std::cout << char_of_color(b[color_land::position{row, column}]) << " ";
    }
    std::cout << "]\n";
  }
  std::cout << "\n";
}

inline void show_solution(const color_land::solution_board& sb) noexcept {
  for (size_t row = 0; row < sb.rows(); ++row) {
    std::cout << " [ ";
    for (size_t column = 0; column < sb.columns(); ++column) {
      if (column > 0) {
        std::cout << "| ";
      }
      if (sb[color_land::position{row, column}].s ==
          color_land::solution::shown) {
        std::cout << sb[color_land::position{row, column}].enclosure_number
                  << " ";
      } else {
        std::cout << "  ";
      }
    }
    std::cout << "]\n";
  }
  std::cout << "\n";
}

int main() {
  std::srand(std::time(nullptr));
  color_land::board b{7, 7};
  fill_board(b, 8);
  auto sb = generate_solution(b);
  show_board(b);
  show_solution(sb);
}