cmake_minimum_required(VERSION 3.22)

# Standalone profiler for string move performance comparison
add_executable(profile_string_moves ProfileStringMoves.cpp)

target_compile_options(profile_string_moves PRIVATE
    -O3
    -march=native
)
