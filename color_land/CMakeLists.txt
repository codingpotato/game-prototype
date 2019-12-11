add_executable(color_land color_land.cpp)
target_compile_features(color_land PRIVATE cxx_std_17)
target_compile_options(color_land PRIVATE -Werror -Wall -Wextra)

add_executable(level_generator_color_land level_generator.cpp)
target_compile_features(level_generator_color_land PRIVATE cxx_std_17)
target_compile_options(level_generator_color_land PRIVATE -Werror -Wall -Wextra)

add_subdirectory(tests)
