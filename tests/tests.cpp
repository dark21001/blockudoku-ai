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

	BOOST_ASSERT(BitBoard(1, 3).count() == 3);

	std::cout << BitBoard::full().count() << std::endl;
	BOOST_ASSERT(BitBoard::full().count() == 9*9);
}

int main()
{
	testBitBoardBasics();
	return 0;
}

