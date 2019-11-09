#include <iostream>

#include "board.hpp"

int main() {
  std::srand(std::time(nullptr));
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  const auto& [ps1, ps2] = match_same(board);
  for (auto position : ps1) {
    std::cout << "{" << position.row << ", " << position.column << "} ";
  }
  std::cout << "\n";
  for (auto position : ps2) {
    std::cout << "{" << position.row << ", " << position.column << "} ";
  }
  std::cout << "\n";
}