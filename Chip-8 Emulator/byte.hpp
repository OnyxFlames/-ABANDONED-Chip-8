#ifndef ONYXLIB_BYTE_HPP
#define ONYXLIB_BYTE_HPP

#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>

namespace Onyx
{
	typedef uint8_t byte;

	int16_t to_short(std::array<byte, 2> _bytes);
	uint16_t to_ushort(std::array<byte, 2> _bytes);
	int32_t to_int(std::array<byte, 4> _bytes);
	uint32_t to_uint(std::array<byte, 4> _bytes);
	int64_t to_long(std::array<byte, 8> _bytes);
	uint64_t to_ulong(std::array<byte, 8> _bytes);
	float to_float(std::array<byte, 4> _bytes);
	double to_double(std::array<byte, 8> _bytes);
	byte get_left(byte _byte);
	byte get_right(byte _byte);
	byte get_byte(std::istream &stream = std::cin);
}


#endif