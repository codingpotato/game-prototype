list(APPEND CMAKE_MODULE_PATH "${catch_SOURCE_DIR}/contrib")
include(CTest)
include(ParseAndAddCatchTests)

function(add_test_executable)
  cmake_parse_arguments(ADD
                        OPTIONS
                        NAME
                        SOURCES
                        ${ARGN})
  add_executable(${ADD_NAME}_color_fill ${ADD_SOURCES})
  target_compile_features(${ADD_NAME}_color_fill PUBLIC cxx_std_17)
  target_compile_options(${ADD_NAME}_color_fill
                         PRIVATE -Wall
                                 -Wextra
                                 -Werror
                                 -Wpedantic
                                 -pedantic-errors)
  target_include_directories(${ADD_NAME}_color_fill
                             PUBLIC .. ${catch_SOURCE_DIR}/single_include)
  parseandaddcatchtests(${ADD_NAME}_color_fill)
endfunction(add_test_executable)

add_test_executable(NAME
                    board_tests
                    SOURCES
                    board_tests.cpp)
add_test_executable(NAME
                    matrix_tests
                    SOURCES
                    matrix_tests.cpp)
