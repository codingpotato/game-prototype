#pragma once

#include <algorithm>
#include <iterator>
#include <map>
#include <optional>
#include <set>

#include "matrix.hpp"

namespace color_land {

struct color {
  int raw_value = null_value_;

  constexpr static color null() noexcept { return color{}; }
  constexpr static color stone() noexcept { return color{stone_value_}; }

  constexpr color() noexcept {}
  constexpr color(int value) noexcept : raw_value{value} {}

  constexpr bool is_null() const noexcept { return raw_value == null_value_; }
  constexpr bool is_stone() const noexcept { return raw_value == stone_value_; }
  constexpr bool is_color() const noexcept {
    return raw_value != null_value_ && raw_value != stone_value_;
  }

  friend bool operator==(color lhs, color rhs) noexcept {
    return lhs.raw_value == rhs.raw_value;
  }

  friend bool operator<(color lhs, color rhs) noexcept {
    return lhs.raw_value < rhs.raw_value;
  }

 private:
  static constexpr int null_value_ = -1;
  static constexpr int stone_value_ = 0;
};

struct puzzle {
  enum status { unknown, shown, hidden };
  status stat = unknown;
};

struct information {
  int enclosure_number = 0;
  std::set<color> impossible_colors;
};

using board = matrix<color>;
using puzzle_board = matrix<puzzle>;
using information_board = matrix<information>;

struct candidate {
  candidate(position p) noexcept : pos{p} {}

  position pos;
  bool is_difficult = false;
  positions shown_positions;
  positions hidden_positions;
};

using candidates = std::vector<candidate>;
using position_pair = std::pair<position, position>;

template <typename T>
inline std::optional<T> random_element_of(std::vector<T> v) noexcept {
  if (v.empty()) {
    return {};
  }
  return v[std::rand() % v.size()];
}

inline void fill_stones(board &b, size_t stones) noexcept {
  if (stones == 0) {
    return;
  }
  for (size_t stone = 0; stone < stones; ++stone) {
    auto ps = all_positions_if(b.begin(), b.end(), [&](position pos, color) {
      auto nv = b.neighber_view_of(pos, neighber_type::all);
      auto count = std::count(nv.begin(), nv.end(), color::stone());
      return pos.row == 0 || pos.column == 0 ||
                     pos.row == static_cast<int>(b.rows()) - 1 ||
                     pos.column == static_cast<int>(b.columns()) - 1
                 ? count == 0
                 : count <= 1;
    });
    if (auto pos = random_element_of(ps)) {
      b[*pos] = color::stone();
    }
  }
}

inline std::optional<position_pair> random_connected_empty_positions(
    board &b) noexcept {
  auto ps = all_positions_if(b.begin(), b.end(), [&](position pos, color c) {
    auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
    return c.is_null() && std::count(nv.begin(), nv.end(), color::null()) > 0;
  });
  if (auto pos = random_element_of(ps)) {
    auto nv = b.neighber_view_of(*pos, neighber_type::no_diagonal);
    auto neighbers = all_positions(nv.begin(), nv.end(), color::null());
    if (auto neighber = random_element_of(neighbers)) {
      return std::make_optional<position_pair>(*pos, *neighber);
    }
  }
  return {};
}

inline bool fill_two_connected_seeds(board &b, color c) noexcept {
  if (auto neighber_pair = random_connected_empty_positions(b)) {
    auto [pos, neighber] = *neighber_pair;
    b[pos] = c;
    b[neighber] = c;
    return true;
  }
  return false;
}

inline void fill_seeds_in_board(board &b, int colors) noexcept {
  for (auto value = 1; value <= colors; ++value) {
    fill_two_connected_seeds(b, color{value});
  }
}

inline std::vector<std::pair<color, int>> sorted_color_counts(
    board &b) noexcept {
  std::map<color, int> counts_map;
  color_land::for_each(b.begin(), b.end(), [&](position, color c) {
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
  std::sort(counts.begin(), counts.end(), [](const auto &lhs, const auto &rhs) {
    return lhs.second < rhs.second;
  });
  return counts;
}

inline std::vector<std::pair<position, positions>>
positions_with_empty_neighbers(board &b, color expected_color) noexcept {
  std::vector<std::pair<position, positions>> results;
  color_land::for_each(b.begin(), b.end(), [&](position pos, color c) {
    if (c == expected_color) {
      auto nv = b.neighber_view_of(pos, neighber_type::no_diagonal);
      auto empty_neighbers = all_positions(nv.begin(), nv.end(), color::null());
      if (empty_neighbers.size() > 0) {
        results.emplace_back(pos, empty_neighbers);
      }
    }
  });
  std::sort(results.begin(), results.end(),
            [](const auto &lhs, const auto &rhs) {
              return lhs.second.size() > rhs.second.size();
            });
  return results;
}

inline void fill_board(board &b, size_t colors, size_t stones) noexcept {
  fill_stones(b, stones);
  fill_seeds_in_board(b, colors);
  auto has_empty_neighbers = true;
  while (has_empty_neighbers) {
    has_empty_neighbers = false;
    for (auto color_count : sorted_color_counts(b)) {
      auto positionsAndNeighbers =
          positions_with_empty_neighbers(b, color_count.first);
      if (positionsAndNeighbers.size() > 0) {
        b[*random_element_of(positionsAndNeighbers[0].second)] =
            b[positionsAndNeighbers[0].first];
        has_empty_neighbers = true;
        break;
      }
    }
  }
}

inline void calculate_enclosure_number(board &b,
                                       information_board &ib) noexcept {
  for (auto it = b.begin(); it != b.end(); ++it) {
    if (it->is_color()) {
      auto pos = it.pos();
      auto nv = b.neighber_view_of(pos, neighber_type::all);
      ib[pos].enclosure_number = std::count(nv.begin(), nv.end(), b[pos]);
    }
  }
}

inline int known_enclosure_number(board &b, puzzle_board &pb,
                                  position pos) noexcept {
  auto nv = b.neighber_view_of(pos, neighber_type::all);
  auto count = 0;
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    if (pb[it.pos()].stat != puzzle::unknown && *it == b[pos]) {
      ++count;
    }
  }
  return count;
}

inline candidate candidate_of(board &b, puzzle_board &pb,
                              const information_board &ib,
                              position pos) noexcept {
  candidate result{pos};
  if (pb[pos].stat == puzzle::unknown) {
    result.shown_positions.emplace_back(pos.row, pos.column);
  }
  auto type = ib[pos].enclosure_number - known_enclosure_number(b, pb, pos) == 1
                  ? neighber_type::no_diagonal
                  : neighber_type::all;
  auto nv = b.neighber_view_of(pos, type);
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    auto neighber_pos = it.pos();
    if (pb[neighber_pos].stat == puzzle::unknown) {
      if (*it == b[pos]) {
        result.hidden_positions.emplace_back(neighber_pos.row,
                                             neighber_pos.column);
      } else {
        if (ib[neighber_pos].impossible_colors.find(b[pos]) ==
            ib[neighber_pos].impossible_colors.end()) {
          result.shown_positions.emplace_back(neighber_pos.row,
                                              neighber_pos.column);
        } else {
          result.is_difficult = true;
        }
      }
    }
  }
  return result;
}

inline candidates calculate_candidates(board &b, puzzle_board &pb,
                                       const information_board &ib) noexcept {
  candidates result;
  for (auto it = b.begin(); it != b.end(); ++it) {
    if (auto cand = candidate_of(b, pb, ib, it.pos());
        cand.shown_positions.size() > 0 || cand.hidden_positions.size() > 0) {
      result.push_back(std::move(cand));
    }
  }
  std::sort(
      result.begin(), result.end(),
      [](const candidate &lhs, const candidate &rhs) {
        return lhs.shown_positions.size() < rhs.shown_positions.size() ||
               (lhs.shown_positions.size() == rhs.shown_positions.size() &&
                lhs.is_difficult && !rhs.is_difficult);
      });
  return result;
}

inline void update_information(board &b, puzzle_board &pb,
                               information_board &ib, position pos) noexcept {
  if (ib[pos].enclosure_number == known_enclosure_number(b, pb, pos)) {
    auto nv = b.neighber_view_of(pos, neighber_type::all);
    for (auto it = nv.begin(); it != nv.end(); ++it) {
      if (pb[it.pos()].stat == puzzle::unknown) {
        ib[it.pos()].impossible_colors.insert(b[pos]);
      }
    }
  }
}

inline void update_neighber_information(board &b, puzzle_board &pb,
                                        information_board &ib,
                                        position pos) noexcept {
  auto nv = b.neighber_view_of(pos, neighber_type::all);
  for (auto it = nv.begin(); it != nv.end(); ++it) {
    update_information(b, pb, ib, it.pos());
  }
}

inline std::optional<candidate> random_less_shown_candidate(
    const candidates &cs, bool is_difficult) noexcept {
  if (cs.empty()) {
    return {};
  }
  size_t index = 1;
  while (index < cs.size() &&
         cs[index].shown_positions.size() <= cs[0].shown_positions.size() &&
         (!is_difficult || cs[index].is_difficult)) {
    ++index;
  }
  return cs[std::rand() % index];
}

inline void set_shown_status_for_stones(board &b, puzzle_board &pb) noexcept {
  b.for_each([&](position pos, color c) {
    if (c.is_stone()) {
      pb[pos].stat = puzzle::shown;
    }
  });
}

inline puzzle_board generate_puzzle(board &b, bool is_difficult) noexcept {
  puzzle_board pb{b.rows(), b.columns()};
  information_board ib{b.rows(), b.columns()};
  set_shown_status_for_stones(b, pb);
  calculate_enclosure_number(b, ib);
  while (true) {
    auto cs = calculate_candidates(b, pb, ib);
    if (auto candidate = random_less_shown_candidate(cs, is_difficult)) {
      for (auto pos : candidate->shown_positions) {
        pb[pos].stat = puzzle::shown;
        update_neighber_information(b, pb, ib, pos);
      }
      for (auto pos : candidate->hidden_positions) {
        pb[pos].stat = puzzle::hidden;
        update_neighber_information(b, pb, ib, pos);
      }
    } else {
      break;
    }
  }
  return pb;
}

}  // namespace color_land
