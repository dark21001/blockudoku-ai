
add_executable(blockudoku-solver-tests "tests.cpp" "../engine.h" "../engine.cpp" "boostassert.h")
add_test(tester blockudoku-solver-tests)