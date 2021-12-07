
cmake_minimum_required (VERSION 3.8)

project ("blockudoku-ai")

# Add source to this project's executable.
add_executable (blockudoku-ai "blockudoku-ai.cpp" "engine.h" "engine.cpp")

enable_testing()
add_subdirectory(tests)
