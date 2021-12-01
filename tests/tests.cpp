#pragma once
#include <stdlib.h>
#include "../engine.h"
#include "boostassert.h"

void testBitBoardBasics() {
	BOOST_ASSERT(!BitBoard::empty());

	// All locations are empty.
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			BOOST_ASSERT(!BitBoard::empty().at(r, c));
		}
	}

}

int main()
{
	testBitBoardBasics();
	return 0;
}

