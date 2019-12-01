#pragma once

#include <algorithm>
#include <optional>
#include <set>

#include "matrix.hpp"

using color = size_t;

inline color null_color() noexcept { return 0; }
inline bool is_null(color c) noexcept { return c == null_color(); }

using board = matrix<color>;

struct solution {
  enum status { unknown, shown, hidden };

  int enclosure_number = 0;
  status s = unknown;
};

using solution_board = matrix<solution>;

struct information {
  std::set<color> impossible_colors;
};

using information_board = matrix<information>;

struct candidate {
  candidate(int r, int c) noexcept : row{r}, column{c} {}

  int row;
  int column;
  positions shown_positions;
  positions hidden_positions;
};

using candidates = std::vector<candidate>;

using position_pair = std::pair<position, position>;

inline std::optional<position_pair> random_empty_neighber_pair(
    board& b) noexcept {
  auto ps = b.all_positions_if([&b](position pos, color c) {
    auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
    return is_null(c) && std::count(nv.begin(), nv.end(), null_color()) > 0;
  });
  if (ps.empty()) {
    return {};
  }

  auto pos = ps[std::rand() % ps.size()];
  auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
  positions neighbers;
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    if (is_null(*it)) {
      neighbers.push_back(it.pos());
    }
  }
  return std::make_optional<position_pair>(
      pos, neighbers[std::rand() % neighbers.size()]);
}

inline bool fill_two_neighber_pairs(board& b, color c) noexcept {
  if (auto neighber_pair = random_empty_neighber_pair(b)) {
    auto [pos, neighber] = *neighber_pair;
    b[pos] = c;
    b[neighber] = c;
    return true;
  }
  return false;
}

inline void initialize_board(board& b, size_t colors) noexcept {
  for (color c = 1; c <= colors; ++c) {
    fill_two_neighber_pairs(b, c);
  }
}

inline void fill_board(board& b, size_t colors) noexcept {
  initialize_board(b, colors);
  while (true) {
    auto ps = b.all_positions_if([&b](position pos, color c) {
      auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
      return is_null(c) && std::count_if(nv.begin(), nv.end(), [](color c) {
                             return !is_null(c);
                           }) > 0;
    });
    if (ps.empty()) {
      break;
    }
    auto pos = ps[std::rand() % ps.size()];
    auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
    std::vector<color> neighbers;
    std::copy_if(nv.begin(), nv.end(), std::back_inserter(neighbers),
                 [](color c) { return !is_null(c); });
    b[pos] = neighbers[std::rand() % neighbers.size()];
  }
}

inline void calculate_enclosure_number(board& b, solution_board& sb) noexcept {
  for (auto it = b.begin(); it != b.end(); ++it) {
    auto pos = it.pos();
    auto nv = b.neighber_view_of(pos, neighber_type::diagonal);
    sb[pos].enclosure_number = std::count_if(
        nv.begin(), nv.end(), [&b, &pos](color c) { return c == b[pos]; });
  }
}

inline int known_enclosure_number(board& b, solution_board& sb,
                                  position pos) noexcept {
  auto nv = b.neighber_view_of(pos, neighber_type::diagonal);
  auto count = 0;
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    if (sb[it.pos()].s != solution::unknown && *it == b[pos]) {
      ++count;
    }
  }
  return count;
}

inline candidate candidate_of(board& b, solution_board& sb,
                              const information_board& ib,
                              position pos) noexcept {
  candidate s{pos.row, pos.column};
  if (sb[pos].s == solution::unknown) {
    s.shown_positions.emplace_back(pos.row, pos.column);
  }
  auto t = sb[pos].enclosure_number - known_enclosure_number(b, sb, pos) == 1
               ? neighber_type::no_diagonal
               : neighber_type::diagonal;
  auto nv = b.neighber_view_of(pos, t);
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    auto neighber_pos = it.pos();
    if (sb[neighber_pos].s == solution::unknown) {
      if (*it == b[pos]) {
        s.hidden_positions.emplace_back(neighber_pos.row, neighber_pos.column);
      } else if (ib[neighber_pos].impossible_colors.find(b[pos]) ==
                 ib[neighber_pos].impossible_colors.end()) {
        s.shown_positions.emplace_back(neighber_pos.row, neighber_pos.column);
      }
    }
  }
  return s;
}

inline candidates get_candidates(board& b, solution_board& sb,
                                 const information_board& ib) noexcept {
  candidates ss;
  for (auto it = b.begin(); it != b.end(); ++it) {
    if (auto s = candidate_of(b, sb, ib, it.pos());
        s.shown_positions.size() > 0 || s.hidden_positions.size() > 0) {
      ss.emplace_back(s);
    }
  }
  std::sort(ss.begin(), ss.end(), [](const candidate& s1, const candidate& s2) {
    return s1.shown_positions.size() < s2.shown_positions.size() ||
           (s1.shown_positions.size() == s2.shown_positions.size() &&
            s1.hidden_positions.size() > s2.hidden_positions.size());
  });
  return ss;
}

inline void update_information(board& b, solution_board& sb,
                               information_board& ib, position pos) noexcept {
  if (sb[pos].enclosure_number == known_enclosure_number(b, sb, pos)) {
    auto nv = b.neighber_view_of(pos, neighber_type::diagonal);
    for (auto it = nv.begin(); it != nv.end(); ++it) {
      if (sb[it.pos()].s == solution::unknown) {
        ib[it.pos()].impossible_colors.insert(b[pos]);
      }
    }
  }
}

inline void update_neighber_information(board& b, solution_board& sb,
                                        information_board& ib,
                                        position pos) noexcept {
  auto nv = b.neighber_view_of(pos, neighber_type::diagonal);
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    update_information(b, sb, ib, it.pos());
  }
}

inline solution_board generate_solution(board& b) noexcept {
  solution_board sb{b.rows(), b.columns()};
  calculate_enclosure_number(b, sb);
  information_board ib{b.rows(), b.columns()};
  while (true) {
    auto cs = get_candidates(b, sb, ib);
    if (cs.empty()) {
      break;
    }
    for (auto pos : cs[0].shown_positions) {
      sb[pos].s = solution::shown;
      update_neighber_information(b, sb, ib, pos);
    }
    for (auto pos : cs[0].hidden_positions) {
      sb[pos].s = solution::hidden;
      update_neighber_information(b, sb, ib, pos);
    }
  }
  return sb;
}
