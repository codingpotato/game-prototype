add_executable(color_land main.cpp)
target_compile_features(color_land PRIVATE cxx_std_17)
target_compile_options(color_land PRIVATE -Werror -Wall -Wextra)

add_subdirectory(tests)
