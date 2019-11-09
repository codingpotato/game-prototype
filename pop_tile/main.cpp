#include <iostream>

#include "board.hpp"

void game_loop() noexcept {
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  while (match_same(board) > 0) {
    std::cout << board << "\n";
  }
  if (is_game_over(board)) {
    std::cout << "Game over!\n";
  }
  do {
    generate_new_row(board);
    std::cout << "Generate new row:\n" << board << "\n";
    while (match_same(board) > 0) {
      std::cout << board << "\n";
    }
  } while (board.empty());
}

int main() {
  std::srand(std::time(nullptr));
  game_loop();
}