// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "engine.h"
#include <iostream>

using namespace std;

void printPieceSet(PieceSet piece_set) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 6; k++) {
				cout << (piece_set.pieces[j].getBitBoard().at(i, k) ? '#' : ' ');
			}
		}
		cout << endl;
	}
}

int main() {
	srand((unsigned)time(NULL));

	auto game = GameState(BitBoard::empty());
	int turns = 0;
	while (true) {
		turns++;
		cout << "===== TURN " << turns << endl;
		cout << game.getBitBoard().str() << endl;

		const auto piece_set = PieceSet::getRandom();
		printPieceSet(piece_set);
		cout << endl;

		const auto next = AI::makeMoveLookahead(game, piece_set);
		if (next.getBitBoard() == BitBoard::full()) {
			break;
		}
		game = next;
	}
	cout << "Game over!" << endl;
	return 0;
}
