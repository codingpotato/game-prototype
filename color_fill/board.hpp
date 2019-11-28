#pragma once

#include <matrix.hpp>

struct tile {
  size_t color;
};

using board = matrix<tile>;