#pragma once
#include <cstdint>
#include <string>


class BitBoard {
private:
	uint64_t a, b;
	
public:
	explicit BitBoard(uint64_t a, uint64_t b);
	operator bool() const;
	bool operator==(BitBoard other) const;
	bool operator<(BitBoard other) const;
	static BitBoard empty();
	static BitBoard full();

	static BitBoard row(unsigned r);
	static BitBoard column(unsigned c);
	static BitBoard cube(unsigned r, unsigned c);

	bool at(unsigned r, unsigned c) const;

	BitBoard operator|(const BitBoard other) const;
	BitBoard operator&(const BitBoard other) const;
	BitBoard operator-(const BitBoard other) const;

	BitBoard shiftRight() const;
	BitBoard shiftDown() const;

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

public:
	explicit GameState(BitBoard bb);
	BitBoard getBitBoard() const;
	NextGameStateIteratorGenerator nextStates(Piece piece) const;
	double simpleEval() const;
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
	static GameState makeMoveSimple(GameState state, Piece p1, Piece p2, Piece p3);
	static GameState makeMoveLookhead(GameState state, Piece p1, Piece p2, Piece p3);
};