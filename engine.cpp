#include "engine.h"
#include <cassert>
#include <bitset>

namespace {
	const uint64_t ROW_0 = 0x1FFULL;
	const uint64_t TOP_LEFT_CUBE = 0x7ULL | (0x7ULL << 9) | (0x7ULL << 18);
	const uint64_t ALL_ALLOWED_BITS_IN_A = 0x3FFFFFFFFFFFFFULL;
	const uint64_t ALL_ALLOWED_BITS_IN_B = 0x7FFFFFFULL;
	const uint64_t RIGHT_MOST_COLUMN_B = (1ULL << 8) | (1ULL << 17) | (1ULL << 26);
	const uint64_t RIGHT_MOST_COLUMN_A = RIGHT_MOST_COLUMN_B
		 | (1ULL << 35) | (1ULL << 44) | (1ULL << 53);
	const uint64_t ROW_5 = 0x1FFULL << (5 * 9);
}

// === BIT BOARD

bool BitBoard::at(unsigned r, unsigned c) const {
	assert(r < 9);
	assert(c < 9);
	if (r <= 5) {
		return (1ULL << (r * 9 + c)) & a;
	}
	else {
		return (1ULL << ((r - 6) * 9 + c)) & b;
	}
}

BitBoard BitBoard::operator|(const BitBoard other) const {
	return BitBoard(a | other.a, b | other.b);
}

BitBoard BitBoard::operator&(const BitBoard other) const {
	return BitBoard(a & other.a, b & other.b);
}

BitBoard BitBoard::operator-(const BitBoard other) const {
	return BitBoard(a &~other.a, b &~other.b);
}

bool BitBoard::operator==(const BitBoard other) const {
	return a == other.a && b == other.b;
}

BitBoard BitBoard::empty() {
	return BitBoard(0, 0);
}

BitBoard BitBoard::full() {
	return BitBoard(ALL_ALLOWED_BITS_IN_A, ALL_ALLOWED_BITS_IN_B);
}

BitBoard BitBoard::row(unsigned r) {
	assert(r < 9);
	if (r <= 5) {
		return BitBoard(ROW_0 << (r * 9), 0);
	}
	else {
		return BitBoard(0, ROW_0 << ((r - 6)*9));
	}
}

BitBoard BitBoard::column(unsigned c) {
	assert(c < 9);
	int to_shift = 8 - c;
	return BitBoard(RIGHT_MOST_COLUMN_A >> to_shift, RIGHT_MOST_COLUMN_B >> to_shift);
}

BitBoard BitBoard::cube(unsigned r, unsigned c) {
	assert(c < 3);
	assert(r < 3);
	if (r < 2) {
		return BitBoard(TOP_LEFT_CUBE << (3*c + 27*r), 0);
	}
	else {
		return BitBoard(0, TOP_LEFT_CUBE << 3 * c);
	}
}


BitBoard BitBoard::shiftRight() const {
	return BitBoard((a &~ RIGHT_MOST_COLUMN_A) << 1, (b&~RIGHT_MOST_COLUMN_B) << 1);
}

BitBoard BitBoard::shiftDown() const {
	return BitBoard((a << 9) & ALL_ALLOWED_BITS_IN_A, 
		((b << 9) | (a & ROW_5) >> 45) & ALL_ALLOWED_BITS_IN_B);
}


int BitBoard::count() const {
	return (int)std::bitset<64>(a).count() + (int)std::bitset<64>(b).count();
}

BitBoard::operator bool() const {
	return a | b;
}

std::string BitBoard::str() const {
	std::string result;
	for (int r = 0; r < 9; ++r) {
		for (int c = 0; c < 9; ++c) {
			result += at(r, c) ? '#' : '.';
		}
		result += "\n";
	}
	return result;
}

// ====== Piece
Piece::Piece(uint64_t a): bb(BitBoard(a, 0)) {}
BitBoard Piece::getBitBoard() const {
	return bb;
}

// ====== Game State
GameState::GameState(BitBoard bb) : bb(bb) {}
BitBoard GameState::getBitBoard() const {
	return bb;
}


NextGameStateIterator::NextGameStateIterator(GameState state, Piece piece):
	original(state), left(piece.getBitBoard()), next(piece.getBitBoard()) {
}

GameState NextGameStateIterator::operator*() const {
	return GameState(next);
}

/*bool NextGameStateIterator::operator==(NextGameStateIterator other) const {
	return other.left == left;
}*/

void NextGameStateIterator::operator++() {
	if (!(next & BitBoard::column(8))) {
		//next = next.shiftRight();
	} //else 
}

NextGameStateIteratorGenerator::NextGameStateIteratorGenerator(
	GameState state, Piece piece):
	state(state), piece(piece) {}

NextGameStateIterator NextGameStateIteratorGenerator::begin() const {
	return NextGameStateIterator(state, piece);
}

NextGameStateIterator NextGameStateIteratorGenerator::end() const {
	return NextGameStateIterator(state, Piece(BitBoard::empty()));
}