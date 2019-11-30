#include <iostream>

#include "board.hpp"

int main() {
  std::srand(std::time(nullptr));
  board b{7, 7};
  fill_board(b, 7);
  std::cout << b;

  auto sb = generate_solution(b);
  std::cout << sb;
}