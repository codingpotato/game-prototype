#pragma once

#include <algorithm>
#include <iostream>
#include <optional>

#include "matrix.hpp"

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
  for (size_t r = 0; r < m.rows(); ++r) {
    os << " [ ";
    for (size_t c = 0; c < m.columns(); ++c) {
      if (c > 0) {
        os << "| ";
      }
      os << m[position(r, c)] << " ";
    }
    os << "]\n";
  }
  os << "\n";
  return os;
}

using tile = size_t;
using board = matrix<tile>;

struct status {
  enum class type { unknown, shown, hidden };

  int enclosure_number = 0;
  type t = type::unknown;
};

inline std::ostream& operator<<(std::ostream& os, const status& s) {
  switch (s.t) {
    case status::type::unknown:
      os << " ";
      break;
    case status::type::hidden:
      os << "H";
      break;
    case status::type::shown:
      os << "S";
      break;
  }
  return os;
}

using status_board = matrix<status>;

struct solution {
  solution(int r, int c) noexcept : row{r}, column{c} {}

  int row;
  int column;
  positions shown_positions;
  positions hidden_positions;
};

using solutions = std::vector<solution>;

template <typename T>
inline std::optional<std::pair<position, position>> random_empty_neighber_pair(
    matrix<T>& m) noexcept {
  auto ps = m.all_positions_if([&m](position pos, const T& t) {
    if (t != T{}) {
      return false;
    }
    auto nv = m.neighber_view_of(pos, neighber_type::no_diagonal);
    return std::count(nv.begin(), nv.end(), T{}) > 0;
  });
  if (ps.empty()) {
    return {};
  }

  auto pos = ps[std::rand() % ps.size()];
  auto nv = m.neighber_view_of(pos, neighber_type::no_diagonal);
  positions neighbers;
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    if (*it == T{}) {
      neighbers.push_back(it.pos());
    }
  }
  return std::make_optional<std::pair<position, position>>(
      pos, neighbers[std::rand() % neighbers.size()]);
}

inline bool fill_two_neighber_pairs(board& b, size_t color) noexcept {
  if (auto neighber_pair = random_empty_neighber_pair(b)) {
    auto [pos, neighber] = *neighber_pair;
    b[pos] = color;
    b[neighber] = color;
    return true;
  }
  return false;
}

inline void initialize_board(board& b, size_t colors) noexcept {
  for (size_t c = 1; c <= colors; ++c) {
    fill_two_neighber_pairs(b, c);
  }
}

inline void fill_board(board& b, size_t colors) noexcept {
  initialize_board(b, colors);
  while (true) {
    auto ps = b.all_positions_if([&b](position pos, const tile& t) {
      auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
      return t == tile{} &&
             std::count_if(nv.begin(), nv.end(),
                           [](const tile& t) { return t != tile{}; }) > 0;
    });
    if (ps.empty()) {
      break;
    }
    auto pos = ps[std::rand() % ps.size()];
    auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
    std::vector<tile> neighbers;
    std::copy_if(nv.begin(), nv.end(), std::back_inserter(neighbers),
                 [](const tile& t) { return t != tile{}; });
    b[pos] = neighbers[std::rand() % neighbers.size()];
  }
}

inline void calculate_enclosure_number(board& b, status_board& sb) noexcept {
  for (auto it = b.begin(); it != b.end(); ++it) {
    auto pos = it.pos();
    auto nv = b.neighber_view_of(pos, neighber_type::diagonal);
    sb[pos].enclosure_number =
        std::count_if(nv.begin(), nv.end(),
                      [&b, &pos](const tile& t) { return t == b[pos]; });
  }
}

inline solution get_solution(board& b, status_board& sb,
                             position pos) noexcept {
  solution s{pos.row, pos.column};
  if (sb[pos].t == status::type::unknown) {
    s.shown_positions.emplace_back(pos.row, pos.column);
  }
  auto nv = b.neighber_view_of(pos, neighber_type::diagonal);
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    auto neighber_pos = it.pos();
    if (sb[neighber_pos].t == status::type::unknown) {
      if (b[pos] == b[neighber_pos]) {
        s.hidden_positions.emplace_back(neighber_pos.row, neighber_pos.column);
      } else {
        s.shown_positions.emplace_back(neighber_pos.row, neighber_pos.column);
      }
    }
  }
  return s;
}

inline solutions get_solutions(board& b, status_board& sb) noexcept {
  solutions ss;
  for (auto it = b.begin(); it != b.end(); ++it) {
    auto pos = it.pos();
    if (auto s = get_solution(b, sb, pos);
        s.shown_positions.size() > 0 || s.hidden_positions.size() > 0) {
      ss.emplace_back(s);
    }
  }
  std::sort(ss.begin(), ss.end(), [](const solution& s1, const solution& s2) {
    return s1.shown_positions.size() < s2.shown_positions.size() ||
           (s1.shown_positions.size() == s2.shown_positions.size() &&
            s1.hidden_positions.size() > s2.hidden_positions.size());
  });
  return ss;
}

inline status_board generate_solution(board& b) noexcept {
  status_board sb{b.rows(), b.columns()};
  calculate_enclosure_number(b, sb);
  while (true) {
    auto ss = get_solutions(b, sb);
    if (ss.empty()) {
      break;
    }
    for (auto pos : ss[0].shown_positions) {
      sb[pos].t = status::type::shown;
    }
    for (auto pos : ss[0].hidden_positions) {
      sb[pos].t = status::type::hidden;
    }
  }
  return sb;
}