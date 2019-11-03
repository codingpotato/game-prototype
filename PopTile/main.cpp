#include <iostream>

#include "Board.hpp"

int main() {
  std::srand(std::time(nullptr));
  auto board = init_board(20, 4);
  std::cout << board << "\n";
}