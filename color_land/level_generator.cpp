#include <ctime>
#include <iostream>

#include "board.hpp"

inline char char_of_color(color_land::color c) noexcept {
  return 'A' + c.raw_value - 1;
}

inline void show_board(const color_land::board& b,
                       const color_land::solution_board& sb) noexcept {
  b.for_each([](color_land::position, color_land::color c) {
    std::cout << char_of_color(c);
  });
  std::cout << ",";
  sb.for_each([](color_land::position, const color_land::solution& s) {
    std::cout << (s.s == color_land::solution::shown ? 'S' : 'H');
  });
  std::cout << "\n";
}

int main() {
  std::srand(std::time(nullptr));
  for (size_t i = 4; i < 7; ++i) {
    for (auto j = 0; j < 10; ++j) {
      color_land::board b{i, i};
      fill_board(b, 6, 0);
      auto sb = generate_solution(b);
      show_board(b, sb);
    }
  }
}