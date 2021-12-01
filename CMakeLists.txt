
cmake_minimum_required (VERSION 3.8)

project ("blockudoku-solver")

# Add source to this project's executable.
add_executable (blockudoku-solver "blockudoku-solver.cpp" "engine.h" "engine.cpp")

enable_testing()
add_subdirectory(tests)
