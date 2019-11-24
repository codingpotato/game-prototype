add_executable(color_fill main.cpp)
target_compile_features(color_fill PRIVATE cxx_std_17)
target_compile_options(color_fill PRIVATE -Werror -Wall -Wextra)
