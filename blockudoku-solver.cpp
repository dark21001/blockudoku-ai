// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "blockudoku-solver.h"
#include "engine.h"

using namespace std;

int main(){

	for (auto p1 : Piece::getAll()) {

		int numSame = 0;
		for (auto p2 : Piece::getAll()) {
			numSame += p1.getBitBoard() == p2.getBitBoard();
			if (numSame == 3) {
				cout << p1.getBitBoard().str();
			}
		}

	}
	return 0;
}
