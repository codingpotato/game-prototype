#include <ctime>
#include <iostream>
#include <map>

#include "board.hpp"

std::map<color_land::color, char> symbols{
    {color_land::color{1}, 'a'}, {color_land::color{2}, 'b'},
    {color_land::color{3}, 'c'}, {color_land::color{3}, 'd'},
    {color_land::color{3}, 'e'}, {color_land::color{3}, 'f'},
    {color_land::color{3}, 'g'}, {color_land::color{3}, 'h'},
    {color_land::color{3}, 'i'}, {color_land::color{3}, 'j'},
    {color_land::color{3}, 'k'}};

inline void show_board(const color_land::board& b,
                       const color_land::solution_board& sb) noexcept {
  std::cout << "  ";
  for (size_t column = 0; column < b.columns(); ++column) {
    std::cout << "   " << column << "  ";
  }
  std::cout << "\n";
  for (size_t row = 0; row < b.rows(); ++row) {
    std::cout << row << " [ ";
    for (size_t column = 0; column < b.columns(); ++column) {
      if (column > 0) {
        std::cout << "| ";
      }
      color_land::position pos{row, column};
      if (sb[pos].s == color_land::solution::shown) {
        std::cout << symbols[b[pos]] << "," << sb[pos].enclosure_number << " ";
      } else {
        std::cout << "    ";
      }
    }
    std::cout << "]\n";
  }
  std::cout << "\n";
}

inline color_land::color color_of(char ch) noexcept {
  return color_land::color{ch - 'a' + 1};
}

inline void play(const color_land::board& b,
                 color_land::solution_board& sb) noexcept {
  while (true) {
    show_board(b, sb);
    if (std::count_if(sb.begin(), sb.end(), [](const color_land::solution& s) {
          return s.s == color_land::solution::shown;
        }) == static_cast<int>(b.rows() * b.columns())) {
      std::cout << "Done\n";
      break;
    }
    std::cout << "Your guess: ";
    color_land::position pos;
    auto ch = ' ';
    std::cin >> pos.row >> pos.column >> ch;
    if (color_of(ch) == b[pos]) {
      sb[pos].s = color_land::solution::shown;
    } else {
      std::cout << "Fail\n";
      break;
    }
  }
}

int main() {
  std::srand(std::time(nullptr));
  board b{7, 7};
  fill_board(b, 7);

  std::array<int, 8> counts{0, 0, 0, 0, 0, 0, 0, 0};
  for (color c : b) {
    counts[c.raw_value]++;
  }
  std::sort(counts.begin(), counts.end());
  for (auto count : counts) {
    std::cout << count << " ";
  }
  std::cout << "\n";

  auto sb = generate_solution(b);
  play(b, sb);
}