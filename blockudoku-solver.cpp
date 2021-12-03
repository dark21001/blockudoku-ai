// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "blockudoku-solver.h"
#include "engine.h"
#include <algorithm>
#include <vector>

using namespace std;

void printPieceSet(Piece p1, Piece p2, Piece p3) {
	Piece pieces[3] = { p1, p2, p3 };
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 6; k++) {
				cout << (pieces[j].getBitBoard().at(i, k) ? '#' : '.');
			}
		}
		cout << endl;
	}

}

double simpleEvalTest(int numGames) {
	vector<double> scores;
	for (int i = 0; i < numGames; ++i) {
		auto game = GameState(BitBoard::empty());
		int score = 0;
		cout << i << '/' << numGames << endl;
		do {
			score += 1;
			const auto p1 = Piece::getRandom();
			const auto p2 = Piece::getRandom();
			const auto p3 = Piece::getRandom();
			
			game = AI::makeMoveSimple(game,
				p1, p2, p3);
		} while (!game.isOver());
		scores.push_back(score);
	}
	sort(scores.begin(), scores.end());
	return scores[numGames/2];
}

int main(){
	srand(time(NULL));

	auto game = GameState(BitBoard::empty());
	int turns = 0;
	while (true) {
		turns++;
		cout << "===== TURN " << turns << endl;
		cout << game.getBitBoard().str() << endl;
	
		const auto p1 = Piece::getRandom();
		const auto p2 = Piece::getRandom();
		const auto p3 = Piece::getRandom();
		printPieceSet(p1, p2, p3);
		cout << endl;

		game = AI::makeMoveLookhead(game, p1, p2, p3);
		if (game.getBitBoard() == BitBoard::full()) {
			break;
		}
	}
	system("pause");
	return 0;
}
