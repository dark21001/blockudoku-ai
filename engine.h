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
	bool at(unsigned r, unsigned c) const;

	std::string str() const;
};