// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "blockudoku-solver.h"
#include "engine.h"

using namespace std;

int main()
{
	BitBoard acc_cubes = BitBoard::empty();
	cout << (BitBoard::row(3) | BitBoard::column(4)).str() << endl;
	
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			BitBoard cube = BitBoard::cube(r, c);
			acc_cubes = acc_cubes | cube;
			cout << cube.str() << endl;
		}
	}

	cout << "Hello CMake!!\n" << acc_cubes.str() << endl;
	return 0;
}
