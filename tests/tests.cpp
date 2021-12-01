#pragma once
#include <stdlib.h>
#include "../engine.h"
#include "boostassert.h"

int main()
{
	BitBoard B = BitBoard(0, 0);
	BOOST_ASSERT_MSG(1 == 2, "bleh");
	return 0;
}

