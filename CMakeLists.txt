
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("blockudoku-ai")

# Add source to this project's executable.
add_executable (blockudoku-ai "blockudoku-ai.cpp" "engine.h" "engine.cpp")
add_executable (fitness "fitness.cpp" "engine.h" "engine.cpp")

enable_testing()
add_subdirectory(tests)
