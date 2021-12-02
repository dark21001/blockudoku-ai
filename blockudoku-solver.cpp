// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "blockudoku-solver.h"
#include "engine.h"
#include <algorithm>

using namespace std;

int main(){
	srand(time(NULL));
	auto game = GameState(BitBoard::empty());
	int turns = 0;

	while (true) {
		turns++;
		cout << "===== TURN " << turns << endl;
		const auto p1 = Piece::getRandom();
		const auto p2 = Piece::getRandom();
		const auto p3 = Piece::getRandom();
		cout << p1.getBitBoard().str() << endl;
		cout << p2.getBitBoard().str() << endl;
		cout << p3.getBitBoard().str() << endl;

		game = AI::makeMove(game, p1, p2, p3);
		if (game.getBitBoard() == BitBoard::full()) {
			break;
		}

		cout << game.getBitBoard().str() << endl;
	}
	
	cout << "Piece sets " << turns << endl;
	return 0;
}
