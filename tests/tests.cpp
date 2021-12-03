#pragma once
#include <stdlib.h>
#include "../engine.h"
#include "boostassert.h"
#include<iostream>

void assertNumPlacements(
	BitBoard board,
	uint64_t piece,
	int expected) {
	int c = 0;
	for (const GameState x : GameState(board).nextStates(Piece(piece))) {
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
		BOOST_ASSERT(BitBoard::column(r).getHoleCount() == 0);
		BOOST_ASSERT(BitBoard::row(r).getHoleCount() == 0);
	}
	
	BOOST_ASSERT((BitBoard::row(5) | BitBoard::column(4)).count() == 17);
	BitBoard acc_cubes = BitBoard::empty();
	
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			BitBoard cube = BitBoard::cube(r, c);
			BOOST_ASSERT(cube.getHoleCount() == 0);
			BOOST_ASSERT(cube.count() == 9);
			BOOST_ASSERT(cube == (~~cube));
			acc_cubes = (acc_cubes | cube);
		}
	}
	
	BOOST_ASSERT(acc_cubes.count() == 81);
	BOOST_ASSERT((BitBoard::cube(1, 1) - BitBoard::row(4) - BitBoard::column(4)).count() == 4);

	// Basic counting
	BOOST_ASSERT(BitBoard(1, 3).count() == 3);
	std::cout << BitBoard::full().count() << std::endl;
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

	assertNumPlacements(BitBoard::empty(), 1, 81);
	assertNumPlacements(BitBoard::full(), 1, 0);
	assertNumPlacements(BitBoard::full() - BitBoard::column(4), 1, 9);
	assertNumPlacements(BitBoard::empty(), 7, 9*7);
	assertNumPlacements(BitBoard::empty(), 3 | (1<<9), 8*8);

	const BitBoard borders = BitBoard::column(0) | BitBoard::column(8) |
		BitBoard::row(0) | BitBoard::row(8);
	assertNumPlacements(borders, 1, 7*7);
	assertNumPlacements(borders, 3 | (1<<9) | (1<<10), 6 * 6);

	int numPieces = 0;
	int holePieces = 0; // Mini L. XX. U, C
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
		holePieces += p1bb.getHoleCount();
	}
	BOOST_ASSERT(holePieces == 9);
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

	BOOST_ASSERT(BitBoard::empty().getHoleCount() == 0);
	BOOST_ASSERT(BitBoard::full().getHoleCount() == 0);

}

int main()
{
	testBitBoardBasics();
	return 0;
}

