#include "engine.h"
#include <cassert>
#include <bitset>
#include <algorithm>
#include <iostream>

namespace {
	const uint64_t ROW_0 = 0x1FFULL;
	const uint64_t TOP_LEFT_CUBE = 0x7ULL | (0x7ULL << 9) | (0x7ULL << 18);
	const uint64_t ALL_ALLOWED_BITS_IN_A = 0x3FFFFFFFFFFFFFULL;
	const uint64_t ALL_ALLOWED_BITS_IN_B = 0x7FFFFFFULL;
	const uint64_t RIGHT_MOST_COLUMN_B = (1ULL << 8) | (1ULL << 17) | (1ULL << 26);
	const uint64_t RIGHT_MOST_COLUMN_A = RIGHT_MOST_COLUMN_B
		| (1ULL << 35) | (1ULL << 44) | (1ULL << 53);
	const uint64_t LEFT_MOST_COLUMN_A = RIGHT_MOST_COLUMN_A >> 8;
	const uint64_t LEFT_MOST_COLUMN_B = RIGHT_MOST_COLUMN_B >> 8;
	const uint64_t ROW_5 = 0x1FFULL << (5 * 9);
}

// === BIT BOARD

BitBoard::BitBoard(uint64_t a, uint64_t b) : a(a), b(b) {}

bool BitBoard::operator==(BitBoard other) const {
	return a == other.a && b == other.b;
}

bool BitBoard::operator<(BitBoard other) const {
	return a < other.a || (a == other.a && b < other.b);
}

bool BitBoard::at(unsigned r, unsigned c) const {
	return (BitBoard::row(r) & BitBoard::column(c) & (*this));
}

BitBoard BitBoard::operator|(const BitBoard other) const {
	return BitBoard(a | other.a, b | other.b);
}

BitBoard BitBoard::operator&(const BitBoard other) const {
	return BitBoard(a & other.a, b & other.b);
}

BitBoard BitBoard::operator-(const BitBoard other) const {
	return BitBoard(a & ~other.a, b & ~other.b);
}

BitBoard BitBoard::operator~() const {
	return BitBoard((~a) & ALL_ALLOWED_BITS_IN_A, (~b) & ALL_ALLOWED_BITS_IN_B);
}

BitBoard BitBoard::topDownFlip() const {
	auto result = BitBoard::empty();
	for (int r = 0; r < 9; ++r) {
		auto r_mirror = 8 - r;
		auto bits = (*this) & BitBoard::row(r_mirror);
		while (r_mirror != r) {
			if (r_mirror > r) {
				r_mirror--;
				bits = bits.shiftUp();
			}
			else {
				r_mirror++;
				bits = bits.shiftDown();
			}
		}
		result = result | bits;
	}
	return result;
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
		return BitBoard(0, ROW_0 << ((r - 6) * 9));
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
		return BitBoard(TOP_LEFT_CUBE << (3 * c + 27 * r), 0);
	}
	else {
		return BitBoard(0, TOP_LEFT_CUBE << 3 * c);
	}
}


BitBoard BitBoard::shiftRight() const {
	return BitBoard((a & ~RIGHT_MOST_COLUMN_A) << 1, (b & ~RIGHT_MOST_COLUMN_B) << 1);
}

BitBoard BitBoard::shiftLeft() const {
	return BitBoard((a & ~LEFT_MOST_COLUMN_A) >> 1, (b & ~LEFT_MOST_COLUMN_B) >> 1);
}

BitBoard BitBoard::shiftDown() const {
	return BitBoard((a << 9) & ALL_ALLOWED_BITS_IN_A,
		((b << 9) | (a & ROW_5) >> 45) & ALL_ALLOWED_BITS_IN_B);
}

BitBoard BitBoard::shiftUp() const {
	return BitBoard((a >> 9) | ((b & 0x01FFULL) << 45), b >> 9);
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
Piece::Piece(uint64_t a) : bb(BitBoard(a, 0)) {}
Piece::Piece() : bb(BitBoard::empty()) {};
BitBoard Piece::getBitBoard() const {
	return bb;
}


PieceIterator::PieceIterator(uint8_t i) : i(i) {}

PieceIterator PieceIteratorGenerator::begin() const {
	return PieceIterator(0);
}


namespace {

	const uint64_t A = 1ULL << 0;
	const uint64_t B = 1ULL << 1;
	const uint64_t C = 1ULL << 2;
	const uint64_t D = 1ULL << 3;
	const uint64_t E = 1ULL << 4;
	const uint64_t F = 1ULL << 9;
	const uint64_t G = 1ULL << 10;
	const uint64_t H = 1ULL << 11;
	const uint64_t I = 1ULL << 18;
	const uint64_t J = 1ULL << 19;
	const uint64_t K = 1ULL << 20;
	const uint64_t L = 1ULL << 27;
	const uint64_t M = 1ULL << 36;
	/*
	A B C D E
	F G H
	I J K
	L
	M */
	const uint64_t PIECES[] = {
		// 1 square
		A,

		// 2 squares
		A | B,
		A | F,
		A | G,
		B | F,

		// 3 squares
		A | B | C,
		A | F | I,
		A | G | K,
		C | G | I,
		A | B | F,
		A | F | G,
		B | F | G,
		A | B | G,

		// 4 squares
		A | B | C | D,
		A | F | I | L,
		A | B | F | G,

		// L Shape
		A | F | I | J,
		C | F | G | H,
		A | B | G | J,
		A | B | C | F,

		// Flipped L
		A | B | F | I,
		A | B | C | H,
		B | G | I | J,
		A | F | G | H,


		//  X
		// XXX
		B | F | G | H,
		A | F | G | I,
		B | F | G | J,
		A | B | C | G,

		// XX
		//  XX
		A | B | G | H,
		B | F | G | I,
		A | F | G | J,
		B | C | F | G,

		// 5 squares
		A | B | C | D | E, // #####
		A | F | I | L | M,

		A | F | G | H | I, // #
		A | B | C | G | J, // ###
		B | G | I | J | K, // #
		C | F | G | H | K,

		A | B | C | F | H, // ##
		A | B | G | I | J, // #
		A | B | F | I | J, // ##
		A | C | F | G | H,

		A | B | C | F | I, // #
		A | B | C | H | K, // #
		C | H | I | J | K, // ###
		A | F | I | J | K,

		B | F | G | H | J, // + sign
	};
}

Piece Piece::getRandom() {
	return Piece(PIECES[rand() % 47]);
}

bool Piece::operator<(Piece other) const {
	return bb < other.bb;
}

PieceIterator PieceIteratorGenerator::end() const {
	return PieceIterator(sizeof PIECES / sizeof * PIECES);
}

Piece PieceIterator::operator*() const {
	return Piece(PIECES[i]);
}


PieceIteratorGenerator Piece::getAll() {
	return PieceIteratorGenerator();
}

void PieceIterator::operator++() {
	i++;
}

bool PieceIterator::operator!=(PieceIterator other) const {
	return i != other.i;
}

// ====== Game State
GameState::GameState(BitBoard bb) : bb(bb) {}
bool GameState::isOver() const {
	return bb == BitBoard::full();
}
BitBoard GameState::getBitBoard() const {
	return bb;
}
NextGameStateIteratorGenerator GameState::nextStates(Piece piece) const {
	return NextGameStateIteratorGenerator(*this, piece);
}

uint64_t GameState::simpleEvalImpl(BitBoard bb) {
	uint64_t result = 0;

	// 1 point for each block on the board.
	result += bb.count();

	// 1 point for each occupied column.
	for (int i = 0; i < 9; i++) {
		const auto col_bits_a = RIGHT_MOST_COLUMN_A >> i;
		const auto col_bits_b = RIGHT_MOST_COLUMN_B >> i;
		if ((bb.a & col_bits_a) | (bb.b & col_bits_b)) {
			result += 1;
		}
	}

	// 1 point for each occupied row.
	for (int i = 0; i < 6; ++i) {
		const auto row_bits_a = ROW_0 << (i * 9);
		if (row_bits_a & bb.a) {
			result += 1;
		}
	}
	for (int i = 0; i < 3; ++i) {
		const auto row_bits_b = ROW_0 << (i * 9);
		if (row_bits_b & bb.b) {
			result += 1;
		}
	}

	// 3 points for each occupied 3x3 section.
	for (int r = 0; r < 2; r++) {
		for (int c = 0; c < 3; c++) {
			const auto cube_bits = TOP_LEFT_CUBE << (c * 3 + 27 * r);
			if ((bb.a & cube_bits)) {
				result += 3;
			}
			if (r == 0 && (bb.b & cube_bits)) {
				result += 3;
			}
		}
	}

	const auto open = ~bb;

	// 1 point for each open space with no horizontally adjacent open space.
	// #.#
	const auto horizontal_squashed = (open - open.shiftRight() - open.shiftLeft());
	result += horizontal_squashed.count();

	// 1 point for each open space with no vertically adjacent open space.
	// #
	// .
	// #
	const auto verticle_squashed = (open - open.shiftUp() - open.shiftDown());
	result += verticle_squashed.count();

	// 1 point for each open space that has 3 blocks adjacent to it.
	const auto filled_right = bb.shiftLeft();
	const auto filled_left = bb.shiftRight();
	const auto filled_up = bb.shiftDown();
	const auto filled_down = bb.shiftUp();
	const auto open_and_filled_right = open & filled_right;
	const auto open_and_filled_left = open & filled_left;
	result += (open_and_filled_right & filled_down).count();
	result += (open_and_filled_right & filled_up).count();
	result += (open_and_filled_left & filled_down).count();
	result += (open_and_filled_left & filled_up).count();

	return result;
}

uint64_t GameState::simpleEval() const {
	const auto result = simpleEvalImpl(bb);

	assert(bb == bb.topDownFlip().topDownFlip());
	assert(result == simpleEvalImpl(bb.topDownFlip()));

	return result;
}


NextGameStateIterator::NextGameStateIterator(GameState state, Piece piece) :
	original(state), left(piece.getBitBoard()), next(piece.getBitBoard()) {
	if (!canPlace()) {
		operator++();
	}
}

GameState NextGameStateIterator::operator*() const {
	const auto after_add = original.getBitBoard() | next;
	auto to_clear = BitBoard::empty();

	// Clear columns that are completely filled.
	for (int i = 0; i < 9; i++) {
		{
			const auto a_col_bits = RIGHT_MOST_COLUMN_A >> i;
			const auto b_col_bits = RIGHT_MOST_COLUMN_B >> i;
			if ((after_add.a & a_col_bits) == a_col_bits &&
				(after_add.b & b_col_bits) == b_col_bits) {
				to_clear.a |= a_col_bits;
				to_clear.b |= b_col_bits;
			}
		}
	}

	// Clear rows that are completely filled.
	for (int i = 0; i < 6; ++i) {
		const auto row_bits = ROW_0 << (9 * i);
		if ((after_add.a & row_bits) == row_bits) {
			to_clear.a |= row_bits;
		}
	}
	for (int i = 0; i < 3; ++i) {
		const auto row_bits = ROW_0 << (9 * i);
		if ((after_add.b & row_bits) == row_bits) {
			to_clear.b |= row_bits;
		}
	}

	// Clear 3x3s that are completely filled.
	for (int r = 0; r < 2; r++) {
		for (int c = 0; c < 3; c++) {
			const auto cube_bits = TOP_LEFT_CUBE << (c * 3 + 27 * r);
			if ((after_add.a & cube_bits) == cube_bits) {
				to_clear.a |= cube_bits;
			}
			if (r == 0 && (after_add.b & cube_bits) == cube_bits) {
				to_clear.b |= cube_bits;
			}
		}
	}

	return GameState(after_add - to_clear);
}

bool NextGameStateIterator::operator!=(NextGameStateIterator other) const {
	return other.next != next;
}

void NextGameStateIterator::operator++() {
	// We try placing the piece in each row.
	// We try the left most columns first.

	do {
		// We've reached the right edge.
		if (next & BitBoard::column(8)) {

			// We've reached the bottom edge too.
			if (left & BitBoard::row(8)) {
				next = BitBoard::empty();
				break;
			}

			// Start at the first column of the next row.
			left = left.shiftDown();
			next = left;
		}
		else {
			next = next.shiftRight();
		}
	} while (!canPlace());
}


bool NextGameStateIterator::canPlace() const {
	return !(next & original.getBitBoard());
}

NextGameStateIteratorGenerator::NextGameStateIteratorGenerator(
	GameState state, Piece piece) :
	state(state), piece(piece) {
}

NextGameStateIterator NextGameStateIteratorGenerator::begin() const {
	return NextGameStateIterator(state, piece);
}


NextGameStateIterator NextGameStateIteratorGenerator::end() const {
	return NextGameStateIterator(state, Piece(BitBoard::empty()));
}

// ====== AI
GameState AI::makeMoveLookahead(GameState game, Piece p1, Piece p2, Piece p3) {
	Piece pieces[3] = { p1, p2, p3 };
	std::sort(pieces, pieces + 3);

	uint64_t bestScore = 9999999999;
	auto bestNext = GameState(BitBoard::full());

	// If we can clear with 2 pieces or fewer, then we must try permutations.
	bool can_clear_with_2_pieces = false;
	for (int i = 0; i < 3; ++i) {
		for (const auto after_p1 : game.nextStates(pieces[i])) {
			for (int j = 0; j < 3; ++j) {
				if (i == j) {
					continue;
				}
				for (const auto after_p2 : after_p1.nextStates(pieces[j])) {
					if (after_p2.getBitBoard().count() <
						game.getBitBoard().count() + pieces[i].getBitBoard().count() +
						pieces[j].getBitBoard().count()) {
						can_clear_with_2_pieces = true;
						break;
					}
				}
			}
		}
		if (can_clear_with_2_pieces) {
			break;
		}
	}

	// Foreach permutation of the pieces.
	bool is_first_permutation = true;
	do {
		for (const auto after_p1 : game.nextStates(pieces[0])) {
			for (const auto after_p2 : after_p1.nextStates(pieces[1])) {
				for (const auto after_p3 : after_p2.nextStates(pieces[2])) {
					if (!is_first_permutation &&
						after_p3.getBitBoard().count() == game.getBitBoard().count()
						+ pieces[0].getBitBoard().count() +
						pieces[1].getBitBoard().count() +
						pieces[2].getBitBoard().count()
						) {
						// No clears. This position was seen in a previous permutation.
						continue;
					}

					uint64_t total_after_p3 = 0;
					bool is_1x1 = true;
					for (const auto p4 : Piece::getAll()) {
						if (is_1x1) {
							// Be pessimistic and pretend we won't get a 1x1.
							is_1x1 = false;
							continue;
						}

						uint64_t best_after_p4 = 99999999;
						for (const auto after_p4 : after_p3.nextStates(p4)) {
							best_after_p4 = std::min(best_after_p4,
								after_p4.simpleEval());
						}
						total_after_p3 += best_after_p4;
						if (total_after_p3 > bestScore) {
							// after_p3 is worse than the existing candidate already.
							break;
						}
					}

					if (total_after_p3 < bestScore) {
						bestScore = total_after_p3;
						bestNext = after_p3;
					}
				}
			}
		}
		is_first_permutation = false;
	} while (can_clear_with_2_pieces && std::next_permutation(pieces, pieces + 3));

	return bestNext;
}

GameState AI::makeMoveSimple(GameState game, Piece p1, Piece p2, Piece p3) {
	Piece pieces[3] = { p1, p2, p3 };
	std::sort(pieces, pieces + 3);

	uint64_t bestScore = 9999;
	auto bestNext = GameState(BitBoard::full());
	do {
		for (const auto after_p1 : game.nextStates(pieces[0])) {
			for (const auto after_p2 : after_p1.nextStates(pieces[1])) {
				for (const auto after_p3 : after_p2.nextStates(pieces[2])) {
					const auto score = after_p3.simpleEval();
					if (score < bestScore) {
						bestScore = score;
						bestNext = after_p3;
					}
				}
			}
		}
	} while (std::next_permutation(pieces, pieces + 3));

	return bestNext;
}
