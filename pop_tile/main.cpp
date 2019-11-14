#include <iostream>

#include "ai.hpp"

template <bool interactive>
int game_loop() noexcept {
  reset_max_random();
  auto board = init_board(20, 4);
  std::cout << board << "\n";

  auto level = 4;
  auto score_last_level = 0;
  auto score = 0;
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

    if (level < 9 && score - score_last_level > 100) {
      ++level;
      score_last_level = score;
      increase_max_random();
    }

    std::cout << "Score: " << score << "\n";
    if constexpr (interactive) {
      std::cout << "exit: ";
      std::string str;
      std::getline(std::cin, str);
      if (str == "y") {
        break;
      }
    }

    auto pos = predict(board);
    score += remove_neighber(board, pos.row, pos.column);
  }
  return score;
}

int main() {
  std::srand(std::time(nullptr));
  auto max_score = std::numeric_limits<int>::min();
  auto min_score = std::numeric_limits<int>::max();
  auto score_sum = 0;
  auto count = 1000;
  for (auto i = 0; i < count; ++i) {
    auto score = game_loop<false>();
    max_score = std::max(max_score, score);
    min_score = std::min(min_score, score);
    score_sum += score;
  }
  std::cout << "min score: " << min_score << "\n";
  std::cout << "max score: " << max_score << "\n";
  std::cout << "average score: " << score_sum / count << "\n";
}