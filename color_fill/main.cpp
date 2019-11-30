#include <iostream>

#include "board.hpp"

/*std::vector<board> boards{
    {{
        {1, 1, 1, 2},
        {1, 1, 1, 2},
        {1, 1, 1, 2},
        {2, 2, 2, 2},
    }},
    {{
        {1, 1, 2, 2},
        {1, 1, 2, 2},
        {1, 1, 2, 2},
        {1, 1, 2, 2},
    }},
    {{
        {1, 1, 1, 2, 2},
        {1, 1, 1, 2, 2},
        {3, 3, 3, 2, 2},
        {3, 3, 3, 2, 2},
        {3, 3, 3, 2, 2},
    }},
    {{
        {1, 1, 1, 2, 2},
        {1, 1, 1, 2, 2},
        {3, 3, 2, 2, 2},
        {3, 3, 2, 2, 2},
        {3, 3, 2, 2, 2},
    }},
    {{
        {1, 1, 2, 2, 2, 2},
        {1, 1, 2, 2, 2, 2},
        {1, 1, 2, 2, 2, 2},
        {3, 3, 3, 4, 4, 4},
        {3, 3, 3, 3, 4, 4},
        {3, 3, 3, 3, 4, 4},
    }},
    {{
        {1, 1, 1, 1, 1, 1},
        {2, 2, 3, 1, 5, 6},
        {2, 2, 3, 3, 5, 6},
        {2, 2, 3, 3, 5, 6},
        {2, 2, 3, 3, 5, 6},
        {4, 4, 4, 4, 5, 6},
    }},
    {{
        {1, 1, 1, 2, 2, 2},
        {1, 1, 1, 3, 3, 2},
        {4, 1, 1, 1, 3, 2},
        {4, 4, 4, 4, 3, 5},
        {6, 6, 4, 4, 5, 5},
        {6, 6, 6, 6, 6, 6},
    }},
};

struct solution {
  solution(int r, int c) noexcept : row{r}, column{c} {}

  int row;
  int column;
  std::vector<position> shown_positions;
  std::vector<position> hidden_positions;
};

inline solution get_solution(const board& b, const matrix<status>& status_m,
                             position pos) {
  solution s{pos.row, pos.column};
  if (status_m[pos] == status::unknown) {
    s.shown_positions.push_back(pos);
  }
  constexpr size_t direction = 8;
  const std::array<position, direction> moves{
      position{-1, 0}, position{-1, -1}, position{0, -1}, position{1, -1},
      position{1, 0},  position{1, 1},   position{0, 1},  position{-1, 1}};
  for (auto& move : moves) {
    position new_pos{pos.row + move.row, pos.column + move.column};
    if (is_in_matrix(b, new_pos) && status_m[new_pos] == status::unknown) {
      if (b[pos] == b[new_pos]) {
        s.hidden_positions.push_back(new_pos);
      } else {
        s.shown_positions.push_back(new_pos);
      }
    }
  }
  return s;
}

inline std::vector<solution> get_solutions(const board& b,
                                           const matrix<status>& status_m) {
  std::vector<solution> ss;
  for (size_t row = 0; row < b.rows(); ++row) {
    for (size_t column = 0; column < b.columns(); ++column) {
      position pos{static_cast<int>(row), static_cast<int>(column)};
      if (auto s = get_solution(b, status_m, pos);
          s.shown_positions.size() > 0 || s.hidden_positions.size() > 0) {
        ss.push_back(s);
      }
    }
  }
  std::sort(ss.begin(), ss.end(), [](const solution& s1, const solution& s2) {
    return s1.shown_positions.size() < s2.shown_positions.size() ||
           (s1.shown_positions.size() == s2.shown_positions.size() &&
            s1.hidden_positions.size() > s2.hidden_positions.size());
  });
  return ss;
}

inline void print_solutions() noexcept {
  for (auto& b : boards) {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << b;
    auto enclosure = generate_enclosure_board(b);
    std::cout << enclosure;

    matrix<status> status_m{b.rows(), b.columns()};
    while (true) {
      auto solutions = get_solutions(b, status_m);
      if (solutions.size() == 0) {
        break;
      }
      for (auto& pos : solutions[0].shown_positions) {
        status_m[pos] = status::shown;
      }
      for (auto& pos : solutions[0].hidden_positions) {
        status_m[pos] = status::hidden;
      }
    }
    std::cout << status_m;
  }
}*/

int main() {
  std::srand(std::time(nullptr));
  board b{6, 6};
  initialize_board(b, 6);
  std::cout << b;
  /*fill_board(b, 6);
  std::cout << b;

  auto enclosure = generate_enclosure_board(b);
  std::cout << enclosure;

  matrix<status> status_m{b.rows(), b.columns()};
  while (true) {
    auto solutions = get_solutions(b, status_m);
    if (solutions.size() == 0) {
      break;
    }
    for (auto& pos : solutions[0].shown_positions) {
      status_m[pos] = status::shown;
    }
    for (auto& pos : solutions[0].hidden_positions) {
      status_m[pos] = status::hidden;
    }
  }
  std::cout << status_m;*/
}