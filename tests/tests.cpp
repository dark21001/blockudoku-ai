#pragma once
#include <stdlib.h>
#include "../engine.h"
#include "boostassert.h"
#include<iostream>

void testBitBoardBasics() {
	BOOST_ASSERT(!BitBoard::empty());

	// All locations are empty.
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			BOOST_ASSERT(!BitBoard::empty().at(r, c));
			BOOST_ASSERT(BitBoard::full().at(r, c));
		}
	}

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
	BOOST_ASSERT(BitBoard::full().shiftDown().count() == 81 - 9);
	BOOST_ASSERT(BitBoard::full().shiftDown().shiftDown().count() == 81 - 18);
	BOOST_ASSERT(BitBoard::full().shiftDown().shiftRight().count() == 81 - 9 - 8);
}

int main()
{
	testBitBoardBasics();
	return 0;
}

