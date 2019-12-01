#include <iostream>

#include "board.hpp"

constexpr std::array symbols{' ', '@', '#', '$', '%', '&', '*', 'o'};

inline void show_board(const board& b, const solution_board& sb) noexcept {
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
      position pos{row, column};
      if (sb[pos].s == solution::shown) {
        std::cout << symbols[b[pos]] << "," << sb[pos].enclosure_number << " ";
      } else {
        std::cout << "    ";
      }
    }
    std::cout << "]\n";
  }
  std::cout << "\n";
}

inline color color_of(char ch) noexcept {
  return std::distance(symbols.begin(),
                       std::find(symbols.begin(), symbols.end(), ch));
}

inline void play(const board& b, solution_board& sb) noexcept {
  while (true) {
    show_board(b, sb);
    if (std::count_if(sb.begin(), sb.end(), [](const solution& s) {
          return s.s == solution::shown;
        }) == static_cast<int>(b.rows() * b.columns())) {
      std::cout << "Done\n";
      break;
    }
    std::cout << "Your guess: ";
    position pos;
    auto ch = ' ';
    std::cin >> pos.row >> pos.column >> ch;
    if (color_of(ch) == b[pos]) {
      sb[pos].s = solution::shown;
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
  auto sb = generate_solution(b);
  play(b, sb);
}