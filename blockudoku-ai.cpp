// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "engine.h"
#include <iostream>

void printPieceSet(PieceSet piece_set) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 6; k++) {
				std::cout << (piece_set.pieces[j].getBitBoard().at(i, k) ? '#' : ' ');
			}
		}
		std::cout << std::endl;
	}
}

int main() {
	srand((unsigned)time(NULL));

	auto game = GameState(BitBoard::empty());
	int turns = 0;
	while (true) {
		turns++;
		std::cout << "===== TURN " << turns << std::endl;
		std::cout << game.getBitBoard().str() << std::endl;

		const auto piece_set = PieceSet::getRandom();
		printPieceSet(piece_set);
		std::cout << std::endl;

		const auto next = AI::makeMoveLookahead(EvalWeights::getDefault(), game, piece_set);
		if (next.getBitBoard() == BitBoard::full()) {
			break;
		}
		game = next;
	}
	std::cout << "Game over!" << std::endl;
	return 0;
}
