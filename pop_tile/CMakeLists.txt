add_executable(pop_tile main.cpp)
target_compile_features(pop_tile PRIVATE cxx_std_17)
target_compile_options(pop_tile PRIVATE -Werror -Wall -Wextra)

add_subdirectory(tests)
