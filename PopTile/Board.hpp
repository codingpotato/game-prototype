#pragma once

#include <iomanip>
#include <ostream>
#include <vector>

struct Tile {
  int color;
  int number;
};

using Row = std::vector<Tile>;
using Board = std::vector<Row>;

constexpr Tile null_tile = {0, 0};

bool operator==(const Tile& lhs, const Tile& rhs) {
  return lhs.color == rhs.color && lhs.number == rhs.number;
}

bool operator!=(const Tile& lhs, const Tile& rhs) {
  return lhs.color != rhs.color || lhs.number != rhs.number;
}

Tile random_tile(size_t max_number) {
  Tile tile;
  tile.color = std::rand() % (max_number - 1) + 1;
  tile.number = std::rand() % (max_number - 1) + 1;
  return tile;
}

Board init_board(size_t row_count, size_t column_count) {
  Board board(row_count, Row(column_count, null_tile));
  for (size_t row = row_count - 4; row < row_count; ++row) {
    for (auto& tile : board[row]) {
      tile = random_tile(4);
    }
  }
  return board;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
  auto index = 0;
  for (auto& row : board) {
    os << std::setw(2) << index++ << " [ ";
    auto first = true;
    for (auto& tile : row) {
      if (!first) {
        os << "| ";
      } else {
        first = false;
      }
      if (tile != null_tile) {
        os << "C " << tile.color << " N " << tile.number << " ";
      } else {
        os << "        ";
      }
    }
    os << "]\n";
  }
  return os;
}
