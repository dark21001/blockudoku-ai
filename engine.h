#pragma once
#include <cstdint>
#include <string>

class GameState;
class NextGameStateIterator;


class BitBoard {
	// Represents a 9x9 board where each square is 1 or 0.
	// The board has 9 rows numbered 0 <= r < 9.
	// The board has 9 columns numbered 0 <= c < 9.
	// The board has 9 3x3 "cubes" indexed with (r, c). 0 <= r,c < 3.

private:
	uint64_t a, b;
	friend class NextGameStateIterator;
	friend class GameState;
public:
	explicit BitBoard(uint64_t a, uint64_t b);
	operator bool() const;
	bool operator==(BitBoard other) const;
	bool operator<(BitBoard other) const;
	static BitBoard empty();
	static BitBoard full();

	// Return a board with 9 bits in the column/row/cube turned on.
	static BitBoard row(unsigned r);
	static BitBoard column(unsigned c);
	static BitBoard cube(unsigned r, unsigned c);

	// Is the bit at (r, c) on?
	bool at(unsigned r, unsigned c) const;

	// Bitwise operators.
	BitBoard operator|(const BitBoard other) const;
	BitBoard operator&(const BitBoard other) const;
	BitBoard operator~() const;

	// Same as (*this) &~ other.
	BitBoard operator-(const BitBoard other) const;

	// Shift the entire board one space in the direction indicated.
	// When shifting left, the right most column will be empty.
	BitBoard shiftLeft() const;
	BitBoard shiftRight() const;
	BitBoard shiftUp() const;
	BitBoard shiftDown() const;

	BitBoard leastSignificantBit() const;

	// Swap row 0 with 8, 1 with 7, 2 with 6, 3 with 5.
	BitBoard topDownFlip() const;

	// How many bits are 1.
	int count() const;

	std::string str() const;
};

class PieceIteratorGenerator;
class Piece {
private:
	BitBoard bb;

public:
	explicit Piece(uint64_t a);
	Piece();
	BitBoard getBitBoard() const;
	static PieceIteratorGenerator getAll();
	static Piece getRandom();
	bool operator<(Piece other) const;

};

class PieceSet {
public:
	PieceSet(Piece p1, Piece p2, Piece p3);
	static PieceSet getRandom();
	Piece pieces[3];
};

class PieceIterator {
private:
	uint8_t i;
	PieceIterator(uint8_t i);
	friend class PieceIteratorGenerator;
public:

	Piece operator*() const;
	bool operator!=(PieceIterator other) const;
	void operator++();
};

class PieceIteratorGenerator {
private:
	PieceIteratorGenerator() {};
	friend class Piece;
public:
	PieceIterator begin() const;
	PieceIterator end() const;
};


class NextGameStateIteratorGenerator;
class GameState {
private:
	BitBoard bb;
	static uint64_t simpleEvalImpl(BitBoard bb);
public:
	explicit GameState(BitBoard bb);
	BitBoard getBitBoard() const;
	NextGameStateIteratorGenerator nextStates(Piece piece) const;
	uint64_t simpleEval() const;
	bool isOver() const;
};

class NextGameStateIteratorGenerator;
class NextGameStateIterator {
public:

	GameState operator*() const;
	bool operator!=(NextGameStateIterator other) const;
	void operator++();
private:
	explicit NextGameStateIterator(GameState state, Piece piece);
	const GameState original;
	BitBoard next, left;
	bool canPlace() const;
	friend class NextGameStateIteratorGenerator;
};

class NextGameStateIteratorGenerator {
private:
	const GameState state;
	const Piece piece;
	explicit NextGameStateIteratorGenerator(GameState state, Piece piece);
	friend class GameState;
public:
	NextGameStateIterator begin() const;
	NextGameStateIterator end() const;


};

class AI {
public:
	// Return the state with the lowest score after placing the 3 pieces.
	static GameState makeMoveSimple(GameState state, PieceSet piece_set);

	// Similar to makeMoveSimple, but considers possible placements of the 4th piece.
	static GameState makeMoveLookahead(GameState state, PieceSet piece_set);

private:
	static bool canClearWith2PiecesOrFewer(GameState state, PieceSet piece_set);

};
