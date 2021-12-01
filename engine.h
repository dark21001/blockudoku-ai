#pragma once
#include <cstdint>
#include <string>

class BitBoard {
private:
	uint64_t a, b;

public:
	explicit BitBoard(uint64_t a, uint64_t b);

	std::string str() const;
};