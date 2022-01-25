// blockudoku-solver.cpp : Defines the entry point for the application.
//

#include "engine.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <atomic>
#include <thread>
#include <numeric>
#include <cstdlib>

int getNumTurnsSample(EvalWeights weights) {
	auto game = GameState(BitBoard::empty());
	int score = 0;

	while(true) {
		score += 1;
		const auto piece_set = PieceSet::getRandom();
		const auto next = AI::makeMoveSimple(weights, game, piece_set);
		if (next.isOver()){
			break;
		}
		game = next;
	}
	return score;
}

// Call this to test changes to the evaluation function.
double simpleEvalFitnessTest(EvalWeights weights, int numGames) {
	std::atomic<int> games_done(0);
	std::vector<double> scores;
	std::vector<std::thread> workers;
	std::mutex mtx;
    for (int i = 0; i < std::thread::hardware_concurrency(); i++) {
        workers.push_back(std::thread([&]()
        {
			while (games_done++ < numGames){
				const auto score = getNumTurnsSample(weights);
				mtx.lock();
				scores.push_back(score);
				mtx.unlock();
				std::cout << scores.size() << '/' << numGames << ' ' << score << std::endl;
			}
        }));
    }
	std::for_each(workers.begin(), workers.end(), [](std::thread &t)
    {
        t.join();
    });

	std::sort(scores.begin(), scores.end());
	double p50 = scores[numGames / 2];
	std::cout << "p50: " << p50 << std::endl;
	double avg = (std::accumulate(scores.begin(), scores.end(), 0.0) / numGames);
	std::cout << "avg: " << avg << std::endl;

	return avg;
}

std::vector<EvalWeights> getInitialPopulation(int n) {
	/*

[45 32 40 56 72 13 ] 29740
[29 32 40 56 72 13 ] 29089

	*/
	auto parent_a = EvalWeights::fromString("29 32 40 56 72 13");
	std::vector<EvalWeights> results;
	for (int i=0;i<n;++i) {
		results.push_back(parent_a.getMutation());
	}

	return results;
}

void learn(int population_size) {
	auto pop = getInitialPopulation(population_size);
	for (int generation_count=1;;generation_count++){
		std::cout << "=== Generation " << generation_count << std::endl;

		std::vector<std::pair<int, EvalWeights>> scores;
		for (auto w : pop) {
			std::cout << "[" << w.toString() << "]" << std::endl;
			scores.push_back(std::make_pair(simpleEvalFitnessTest(w, 500), w));
		}
		std::cout << std::endl;
		std::sort(scores.begin(), scores.end());
		std::reverse(scores.begin(), scores.end());

		auto parent_a = scores[0];
		auto parent_b = scores[1];
		std::cout << "[" << parent_a.second.toString() << "] " << parent_a.first << std::endl;
		std::cout << "[" << parent_b.second.toString() << "] " << parent_b.first << std::endl;
		std::cout << std::endl;

		pop.clear();
		for (int i=0;i<population_size;i++) {
			pop.push_back(parent_a.second.mate(parent_b.second));
		}
	}
}

int main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));
	learn(8);

	return 0;
}
