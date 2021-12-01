#pragma once
#include <cstdint>
#include <string>

class BitBoard {
private:
	uint64_t a, b;
	
public:
	constexpr explicit BitBoard(uint64_t a, uint64_t b): a(a), b(b){}

	operator bool() const;
	static BitBoard empty();
	static BitBoard full();

	static BitBoard row(unsigned r);
	static BitBoard column(unsigned c);
	static BitBoard cube(unsigned r, unsigned c);

	bool at(unsigned r, unsigned c) const;

	BitBoard operator|(const BitBoard other) const;
	BitBoard operator&(const BitBoard other) const;
	BitBoard operator-(const BitBoard other) const;
	bool operator==(const BitBoard other) const;

	BitBoard shiftRight() const;
	BitBoard shiftDown() const;

	int count() const;

	std::string str() const;
};

class Piece {
private:
	const BitBoard bb;

public:
	explicit Piece(uint64_t a);
	BitBoard getBitBoard() const;
};

class GameState {
private:
	const BitBoard bb;

public:
	explicit GameState(BitBoard bb);
	BitBoard getBitBoard() const;
};

class NextGameStateIterator {
public:
	explicit NextGameStateIterator(GameState state, Piece piece);
	GameState operator*() const;
	bool operator=(NextGameStateIterator other) const;
	void operator++();
private:
	const GameState original;
	BitBoard next, left;
};

class NextGameStateIteratorGenerator {
private:
	const GameState state;
	const Piece piece;
public:
	explicit NextGameStateIteratorGenerator(GameState state, Piece piece);
	NextGameStateIterator begin() const;
	NextGameStateIterator end() const;

};