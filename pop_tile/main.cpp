#include <iostream>

#include "ai.hpp"

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

  auto pos = predict(board);
  std::cout << pos.row << " " << pos.column << "\n";
}

int main() {
  std::srand(std::time(nullptr));
  game_loop();
}