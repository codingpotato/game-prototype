add_executable(PopTile main.cpp)
target_compile_features(PopTile PRIVATE cxx_std_17)
target_compile_options(PopTile PRIVATE -Werror -Wall -Wextra)
