#include "engine.h"
#include <cassert>


BitBoard::BitBoard(uint64_t a, uint64_t b) : a(a), b(b) {}

bool BitBoard::at(unsigned r, unsigned c) const {
	assert(r >= 0);
	assert(c >= 0);
	if (r <= 5) {
		return (1ULL << (r * 9 + c)) & a;
	}
	else {
		return (1ULL << ((r - 5) * 9 + c)) & a;
	}
}

BitBoard BitBoard::empty() {
	return BitBoard(0, 0);
}

BitBoard::operator bool() const {
	return a | b;
}

std::string BitBoard::str() const {
	return "THIS IS STR";
}