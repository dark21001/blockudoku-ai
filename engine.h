#pragma once
#include <cstdint>
#include <string>

class BitBoard {
private:
	uint64_t a, b;
	

public:
	explicit BitBoard(uint64_t a, uint64_t b);
	operator bool() const;
	static BitBoard empty();
	static BitBoard full();
	bool at(unsigned r, unsigned c) const;

	BitBoard operator|(const BitBoard other) const;
	BitBoard operator&(const BitBoard other) const;
	//BitBoard operator~() const;

	int count() const;

	std::string str() const;
};