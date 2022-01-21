#include <stdlib.h>
#include "../engine.h"
#include "boostassert.h"

void assertNumPlacements(
	BitBoard board,
	Piece piece,
	int expected) {
	int c = 0;
	for (const GameState x : GameState(board).nextStates(piece)) {
		c++;
	}
	BOOST_ASSERT(c == expected);
}

void testBitBoardBasics() {
	BOOST_ASSERT(!BitBoard::empty());

	// All locations are empty.
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			BOOST_ASSERT(!BitBoard::empty().at(r, c));
			BOOST_ASSERT(BitBoard::full().at(r, c));
			BOOST_ASSERT(~BitBoard::full() == BitBoard::empty());
			BOOST_ASSERT(BitBoard::full() == ~BitBoard::empty());
		}
	}

	BOOST_ASSERT((BitBoard::row(5) | BitBoard::column(4)).count() == 17);
	BitBoard acc_cubes = BitBoard::empty();

	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			BitBoard cube = BitBoard::cube(r, c);
			BOOST_ASSERT(cube.count() == 9);
			BOOST_ASSERT(cube == (~~cube));
			acc_cubes = (acc_cubes | cube);
		}
	}

	BOOST_ASSERT(acc_cubes.count() == 81);
	BOOST_ASSERT((BitBoard::cube(1, 1) - BitBoard::row(4) - BitBoard::column(4)).count() == 4);

	// Basic counting
	BOOST_ASSERT(BitBoard(1, 3).count() == 3);
	BOOST_ASSERT(BitBoard::full().count() == 9*9);
	BOOST_ASSERT((BitBoard::full()-BitBoard::full()).count() == 0);
	BOOST_ASSERT((BitBoard::empty() - BitBoard::full()).count() == 0);
	BOOST_ASSERT((BitBoard::empty() - BitBoard::empty()).count() == 0);
	BOOST_ASSERT((BitBoard::full() - BitBoard(0xFF, 0xF)).count() == 81 - 12);
	BOOST_ASSERT(BitBoard::full().shiftRight().count() == 81 - 9);
	BOOST_ASSERT(BitBoard::full().shiftRight().shiftRight().count() == 81 - 18);
	BOOST_ASSERT(BitBoard::full().shiftRight().shiftLeft().count() == 81 - 9);
	BOOST_ASSERT(BitBoard::full().shiftLeft().count() == 81 - 9);
	BOOST_ASSERT(BitBoard::full().shiftLeft().shiftLeft().count() == 81 - 18);
	BOOST_ASSERT(BitBoard::full().shiftDown().count() == 81 - 9);
	BOOST_ASSERT(BitBoard::full().shiftDown().shiftDown().count() == 81 - 18);
	BOOST_ASSERT(BitBoard::full().shiftDown().shiftRight().count() == 81 - 9 - 8);
	BOOST_ASSERT(BitBoard::full().shiftUp().count() == 81 - 9);
	BOOST_ASSERT(BitBoard::full().shiftUp().shiftRight().shiftDown()
		.shiftDown().shiftLeft().shiftLeft().count() == 49);

	assertNumPlacements(BitBoard::empty(), Piece(1), 81);
	assertNumPlacements(BitBoard::full(), Piece(1), 0);
	assertNumPlacements(BitBoard::full() - BitBoard::column(4), Piece(1), 9);
	assertNumPlacements(BitBoard::empty(), Piece(7), 9*7);
	assertNumPlacements(BitBoard::empty(), Piece(3 | (1<<9)), 8*8);

	const BitBoard borders = BitBoard::column(0) | BitBoard::column(8) |
		BitBoard::row(0) | BitBoard::row(8);
	assertNumPlacements(borders, Piece(1), 7*7);
	assertNumPlacements(borders, Piece(3 | (1<<9) | (1<<10)), 6 * 6);

	int numPieces = 0;
	for (auto p1 : Piece::getAll()) {
		numPieces++;
		int numSame = 0;
		for (auto p2 : Piece::getAll()) {
			numSame += p1.getBitBoard() == p2.getBitBoard();
		}
		// No duplicates
		BOOST_ASSERT(numSame == 1);

		const auto p1bb = p1.getBitBoard();
		BOOST_ASSERT(p1bb.shiftRight().shiftLeft() == p1bb);
		BOOST_ASSERT(p1bb.shiftDown().shiftUp() == p1bb);
		BOOST_ASSERT(p1bb == (~~p1bb));

		int highestRow = 0, highestCol = 0;
		for (int i = 0; i < 9; ++i) {
			if (p1bb & BitBoard::row(i)) {
				highestRow = i;
			}
			if (p1bb & BitBoard::column(i)) {
				highestCol = i;
			}
		}
		assertNumPlacements(BitBoard::empty(), p1, (9 - highestCol) * (9 - highestRow));
	}
	/*
	 # # #  #    #    #
	## ### #    ##   ##
	             #   #
	 #  ##   #         ##
	###  #  ###       ##
	 #  ##

	*/
	BOOST_ASSERT(numPieces == 47);
	const auto bottom_right_cube = BitBoard::cube(2, 2);
	BOOST_ASSERT(bottom_right_cube.shiftUp().shiftDown() == bottom_right_cube);
	BOOST_ASSERT(bottom_right_cube.shiftLeft().shiftRight() == bottom_right_cube);

	{
		auto b = BitBoard::full();
		auto acc = BitBoard::empty();
		while (b) {
			auto lsb = b.leastSignificantBit();
			BOOST_ASSERT(lsb.count() == 1);
			acc = acc | lsb;
			b = b - lsb;
			BOOST_ASSERT(lsb.getCross().count() == 17);
		}
		BOOST_ASSERT(acc == BitBoard::full());
	}
}

void testWeights() {
	BOOST_ASSERT(EvalWeights::getDefault().toString() ==
	EvalWeights::fromString(EvalWeights::getDefault().toString()).toString());
}

int main()
{
	testBitBoardBasics();
	testWeights();
	return 0;
}
