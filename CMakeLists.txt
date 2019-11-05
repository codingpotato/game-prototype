cmake_minimum_required(VERSION 3.12)

project(
  game-prototype
  VERSION 0.1
  DESCRIPTION "Game Prototype"
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

enable_testing()
include(catch2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(pop_tile)
