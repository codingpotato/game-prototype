#pragma once

#include <iomanip>
#include <ostream>
#include <vector>

struct Tile {
  int color = 0;
  int number = 0;
};

struct Position {
  int row = 0;
  int column = 0;
};

template <typename T>
struct Matrix {
  using Column = std::vector<T>;

  Matrix(size_t rows, size_t columns) noexcept
      : elements(columns, Column(rows, T{})) {}

  const T& at(size_t row, size_t column) const noexcept {
    return elements[column][row];
  }

  T& at(size_t row, size_t column) noexcept { return elements[column][row]; }

 private:
  std::vector<Column> elements;
};

using Row = std::vector<Tile>;
using Board = std::vector<Row>;

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
  Board board(row_count, Row(column_count, Tile{}));
  for (size_t row = row_count - 4; row < row_count; ++row) {
    for (auto& tile : board[row]) {
      tile = random_tile(4);
    }
  }
  return board;
}

std::vector<Position> find_match_3(const Board& board) {
  std::vector<Position> positions;
  for (size_t row = 0; row < board.size(); ++row) {
    auto color_same_count = 1;
    auto number_same_count = 1;
    for (size_t column = 1; column <= board[row].size(); ++column) {
      if (column < board[row].size() && board[row][column] != Tile{} &&
          board[row][column - 1].color == board[row][column].color) {
        color_same_count++;
      } else {
        if (color_same_count >= 3) {
          positions.push_back(
              {static_cast<int>(row), static_cast<int>(column) - 1});
        }
        color_same_count = 1;
      }
      if (column < board[row].size() && board[row][column] != Tile{} &&
          board[row][column - 1].number == board[row][column].number) {
        number_same_count++;
      } else {
        if (number_same_count >= 3) {
          positions.push_back(
              {static_cast<int>(row), static_cast<int>(column) - 1});
        }
        number_same_count = 1;
      }
    }
  }
  for (size_t column = 0; column < board[0].size(); ++column) {
    auto color_same_count = 1;
    auto number_same_count = 1;
    for (size_t row = 1; row <= board.size(); ++row) {
      if (row < board.size() && board[row][column] != Tile{} &&
          board[row - 1][column].color == board[row][column].color) {
        color_same_count++;
      } else {
        if (color_same_count >= 3) {
          positions.push_back(
              {static_cast<int>(row) - 1, static_cast<int>(column)});
        }
        color_same_count = 1;
      }
      if (row < board.size() && board[row][column] != Tile{} &&
          board[row - 1][column].number == board[row][column].number) {
        number_same_count++;
      } else {
        if (number_same_count >= 3) {
          positions.push_back(
              {static_cast<int>(row) - 1, static_cast<int>(column)});
        }
        number_same_count = 1;
      }
    }
  }
  return positions;
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
      if (tile != Tile{}) {
        os << "C " << tile.color << " N " << tile.number << " ";
      } else {
        os << "        ";
      }
    }
    os << "]\n";
  }
  return os;
}
