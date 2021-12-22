// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "engine.h"
#include <algorithm>
#include <vector>
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

// Call this to test changes to the evaluation function.
double simpleEvalFitnessTest(int numGames) {
	vector<double> scores;
	for (int i = 0; i < numGames; ++i) {
		auto game = GameState(BitBoard::empty());
		int score = 0;

		do {
			score += 1;
			const auto piece_set = PieceSet::getRandom();
			game = AI::makeMoveSimple(game, piece_set);

		} while (!game.isOver());
		cout << (i+1) << '/' << numGames << ' ' << score << endl;
		scores.push_back(score);
	}
	sort(scores.begin(), scores.end());
	return scores[numGames / 2];
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
