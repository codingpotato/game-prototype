#include <iostream>

#include "board.hpp"

void game_loop() noexcept {
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  while (match_same(board)) {
    std::cout << board << "\n";
  }
  if (is_game_over(board)) {
    std::cout << "Game over!\n";
  }
  generate_new_row(board);
  std::cout << "Generate new row:\n" << board << "\n";

  while (match_same(board)) {
    std::cout << board << "\n";
  }
}

int main() {
  std::srand(std::time(nullptr));
  game_loop();
}