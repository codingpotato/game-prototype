list(APPEND CMAKE_MODULE_PATH "${catch_SOURCE_DIR}/contrib")
include(CTest)
include(ParseAndAddCatchTests)

function(add_test_executable)
  cmake_parse_arguments(ADD OPTIONS NAME SOURCES ${ARGN})
  add_executable(${ADD_NAME} ${ADD_SOURCES})
  target_compile_features(${ADD_NAME} PUBLIC cxx_std_17)
  target_include_directories(${ADD_NAME}
                             PUBLIC .. ${catch_SOURCE_DIR}/single_include)
  parseandaddcatchtests(${ADD_NAME})
endfunction(add_test_executable)

add_test_executable(NAME board_tests SOURCES board_tests.cpp)
