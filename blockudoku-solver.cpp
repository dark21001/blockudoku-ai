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

		double bestScore = 9999999999;
		auto bestNext = GameState(BitBoard::full());
		for (const auto after_p1 : game.nextStates(p1)) {
			for (const auto after_p2 : after_p1.nextStates(p2)) {
				for (const auto after_p3 : after_p2.nextStates(p3)) {
					double total_after_p3 = 0;
					for (const auto p4 : Piece::getAll()) {
						double best_after_p4 = 99999999;
						for (const auto after_p4 : after_p3.nextStates(p4)) {
							best_after_p4 = std::min(best_after_p4,
								(double)after_p4.simpleEval());
						}
						total_after_p3 += best_after_p4;
						if (total_after_p3 > bestScore) {
							// It already looks shitty.
							break;
						}
					}
					if (total_after_p3 < bestScore) {
						bestScore = total_after_p3;
						bestNext = after_p3;
					}
				}
			}
		}

		if (bestNext.getBitBoard() == BitBoard::full()) {
			break;
		}
		game = bestNext;

		cout << game.getBitBoard().str() << endl;
	}
	
	cout << "Piece sets " << turns << endl;
	return 0;
}
