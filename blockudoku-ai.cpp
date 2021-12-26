// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "engine.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <atomic>
#include <thread>
#include <numeric>

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

int getNumTurnsSample() {
	auto game = GameState(BitBoard::empty());
	int score = 0;

	while(true) {
		score += 1;
		const auto piece_set = PieceSet::getRandom();
		const auto next = AI::makeMoveSimple(game, piece_set);
		if (next.isOver()){
			cout << game.getBitBoard().str() << endl;
			printPieceSet(piece_set);
			break;
		}
		game = next;
	}
	return score;
}

// Call this to test changes to the evaluation function.
double simpleEvalFitnessTest(int numGames) {
	atomic<int> games_done(0);
	vector<double> scores;
	std::vector<std::thread> workers;
    for (int i = 0; i < std::thread::hardware_concurrency(); i++) {
        workers.push_back(std::thread([&]()
        {
			while (games_done++ < numGames){
				const auto score = getNumTurnsSample();
				scores.push_back(score);
				cout << scores.size() << '/' << numGames << ' ' << score << endl;
			}
        }));
    }
	std::for_each(workers.begin(), workers.end(), [](std::thread &t)
    {
        t.join();
    });

	sort(scores.begin(), scores.end());
	double result = scores[numGames / 2];
	cout << "p50: " << result << endl;
	cout << "avg: " << (std::accumulate(scores.begin(), scores.end(), 0.0) / numGames) << endl;

	return result;
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
