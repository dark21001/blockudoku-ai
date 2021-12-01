
add_executable(blockudoku-solver-tests "tests.cpp" "../engine.h" "../engine.cpp")
add_test(tester blockudoku-solver-tests)