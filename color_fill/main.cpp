#include <iostream>

#include "board.hpp"

int main() {
  std::srand(std::time(nullptr));
  board b{7, 7};
  fill_board(b, 7);

  auto sb = generate_solution(b);
  show_board(b, sb);
}