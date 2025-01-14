#include <ctime>
#include <iostream>

#include "board.hpp"

inline char char_of_color(color_land::color c) noexcept {
  return 'A' + c.raw_value - 1;
}

inline void show_board(const color_land::board &b,
                       const color_land::puzzle_board &pb) noexcept {
  b.for_each([](color_land::position, color_land::color c) {
    std::cout << char_of_color(c);
  });
  std::cout << ",";
  pb.for_each([](color_land::position, const color_land::puzzle &puzzle) {
    std::cout << (puzzle.stat == color_land::puzzle::shown ? 'S' : 'H');
  });
  std::cout << "\n";
}

int main() {
  std::srand(std::time(nullptr));
  for (size_t size = 3; size <= 10; ++size) {
    auto game_count = size * size;
    auto min_colors = size - 1;
    auto max_colors =
        std::min(std::max(game_count / 5, size), static_cast<size_t>(12));
    auto color_count = game_count / (max_colors - min_colors + 1);
    auto max_stones = game_count / 10 + 1;
    auto stone_count = color_count / (max_stones + 1);
    for (size_t count = 0; count < game_count; ++count) {
      color_land::board b{size, size};
      auto colors = count / color_count + min_colors;
      auto stones = (count / stone_count) % max_stones;
      auto is_difficult = count % stone_count > stone_count / 2;
      fill_board(b, colors, stones);
      auto pb = generate_puzzle(b, is_difficult);
      show_board(b, pb);
    }
  }
}
