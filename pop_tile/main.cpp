#include <iostream>

#include "ai.hpp"

void game_loop() noexcept {
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  int score = 0;
  while (true) {
    while (auto n = match_same(board)) {
      score += n;
      std::cout << board << "\n";
    }
    if (is_game_over(board)) {
      std::cout << "Game over!\n";
      break;
    }
    do {
      generate_new_row(board);
      std::cout << "Generate new row:\n" << board << "\n";
      while (auto n = match_same(board)) {
        score += n;
        std::cout << board << "\n";
      }
    } while (board.empty());

    std::cout << "Score: " << score << "\n";
    std::cout << "exit: ";
    std::string str;
    std::getline(std::cin, str);
    if (str == "y") {
      break;
    }

    auto pos = predict(board);
    score += remove_neighber(board, pos.row, pos.column);
  }
}

int main() {
  std::srand(std::time(nullptr));
  game_loop();
}