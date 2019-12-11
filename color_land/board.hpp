#pragma once

#include <algorithm>
#include <optional>
#include <set>

#include "matrix.hpp"

namespace color_land {

struct color {
  int raw_value = null_value;

  constexpr static color null() noexcept { return color{}; }
  constexpr static color stone() noexcept { return color{stone_value}; }

  constexpr color() noexcept {}
  constexpr color(int value) noexcept : raw_value{value} {}

  constexpr bool is_null() const noexcept { return raw_value == null_value; }
  constexpr bool is_stone() const noexcept { return raw_value == stone_value; }
  constexpr bool is_color() const noexcept {
    return raw_value != null_value && raw_value != stone_value;
  }

  friend bool operator==(color lhs, color rhs) noexcept {
    return lhs.raw_value == rhs.raw_value;
  }

  friend bool operator<(color lhs, color rhs) noexcept {
    return lhs.raw_value < rhs.raw_value;
  }

 private:
  static constexpr int null_value = -1;
  static constexpr int stone_value = 0;
};

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

template <typename T>
inline std::optional<T> random_element_of_vector(std::vector<T> v) noexcept {
  if (v.empty()) {
    return {};
  } else {
    return v[std::rand() % v.size()];
  }
}

inline void fill_stones(board& b) noexcept {
  auto stones = std::rand() % 5;
  if (stones > 0) {
    for (auto stone = 0; stone < stones; ++stone) {
      auto ps = all_positions_if(b.begin(), b.end(), [&b](position pos, color) {
        auto nv = b.neighber_view_of(pos, neighber_type::all);
        auto count = std::count(nv.begin(), nv.end(), color::stone());
        return pos.row == 0 || pos.column == 0 ||
                       pos.row == static_cast<int>(b.rows()) - 1 ||
                       pos.column == static_cast<int>(b.columns()) - 1
                   ? count == 0
                   : count <= 1;
      });
      if (auto pos = random_element_of_vector(ps)) {
        b[*pos] = color::stone();
      }
    }
  }
}

inline std::optional<position_pair> random_connected_empty_positions(
    board& b) noexcept {
  auto ps = all_positions_if(b.begin(), b.end(), [&b](position pos, color c) {
    auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
    return c.is_null() && std::count(nv.begin(), nv.end(), color::null()) > 0;
  });
  if (auto pos = random_element_of_vector(ps)) {
    auto nv = b.neighber_view_of(*pos, neighber_type::no_diagonal);
    auto neighbers = all_positions(nv.begin(), nv.end(), color::null());
    if (auto neighber = random_element_of_vector(neighbers)) {
      return std::make_optional<position_pair>(*pos, *neighber);
    }
  }
  return {};
}

inline bool fill_two_connected_seeds(board& b, color c) noexcept {
  if (auto neighber_pair = random_connected_empty_positions(b)) {
    auto [pos, neighber] = *neighber_pair;
    b[pos] = c;
    b[neighber] = c;
    return true;
  }
  return false;
}

inline void fill_seeds_in_board(board& b, int colors) noexcept {
  for (auto value = 1; value <= colors; ++value) {
    fill_two_connected_seeds(b, color{value});
  }
}

inline std::vector<std::pair<color, int>> sorted_color_counts(
    board& b) noexcept {
  std::map<color, int> counts_map;
  color_land::for_each(b.begin(), b.end(), [&counts_map](position, color c) {
    if (c.is_color()) {
      if (counts_map.find(c) != counts_map.end()) {
        ++counts_map[c];
      } else {
        counts_map[c] = 1;
      }
    }
  });
  std::vector<std::pair<color, int>> counts;
  std::copy(counts_map.begin(), counts_map.end(), std::back_inserter(counts));
  std::sort(counts.begin(), counts.end(), [](const auto& lhs, const auto& rhs) {
    return lhs.second < rhs.second;
  });
  return counts;
}

inline std::vector<std::pair<position, positions>> positionsWithEmptyNeighbers(
    board& b, color expected_color) noexcept {
  std::vector<std::pair<position, positions>> results;
  color_land::for_each(
      b.begin(), b.end(),
      [&b, expected_color, &results](position pos, color c) {
        if (c == expected_color) {
          auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
          auto empty_neighbers =
              all_positions(nv.begin(), nv.end(), color::null());
          if (empty_neighbers.size() > 0) {
            results.emplace_back(pos, empty_neighbers);
          }
        }
      });
  std::sort(results.begin(), results.end(),
            [](const auto& lhs, const auto& rhs) {
              return lhs.second.size() > rhs.second.size();
            });
  return results;
}

inline void fill_board(board& b, size_t colors) noexcept {
  fill_stones(b);
  fill_seeds_in_board(b, colors);
  auto has_empty_neighbers = true;
  while (has_empty_neighbers) {
    has_empty_neighbers = false;
    for (auto color_count : sorted_color_counts(b)) {
      auto positionsAndNeighbers =
          positionsWithEmptyNeighbers(b, color_count.first);
      if (positionsAndNeighbers.size() > 0) {
        b[*random_element_of_vector(positionsAndNeighbers[0].second)] =
            b[positionsAndNeighbers[0].first];
        has_empty_neighbers = true;
        break;
      }
    }
  }
}

inline void calculate_enclosure_number(board& b, solution_board& sb) noexcept {
  for (auto it = b.begin(); it != b.end(); ++it) {
    auto pos = it.pos();
    auto nv = b.neighber_view_of(pos, neighber_type::all);
    sb[pos].enclosure_number = std::count_if(
        nv.begin(), nv.end(), [&b, &pos](color c) { return c == b[pos]; });
  }
}

inline int known_enclosure_number(board& b, solution_board& sb,
                                  position pos) noexcept {
  auto nv = b.neighber_view_of(pos, neighber_type::all);
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
               : neighber_type::all;
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
    auto nv = b.neighber_view_of(pos, neighber_type::all);
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
  auto nv = b.neighber_view_of(pos, neighber_type::all);
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

}  // namespace color_land
