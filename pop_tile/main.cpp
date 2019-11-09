#include <iostream>

#include "board.hpp"

int main() {
  std::srand(std::time(nullptr));
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  match_same(board);
  std::cout << board << "\n";
}