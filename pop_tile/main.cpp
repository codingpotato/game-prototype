#include <iostream>

#include "Board.hpp"

int main() {
  std::srand(std::time(nullptr));
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  auto positions = find_match_3(board);
  for (auto position : positions) {
    std::cout << "{" << position.row << ", " << position.column << "} ";
  }
  std::cout << "\n";
}