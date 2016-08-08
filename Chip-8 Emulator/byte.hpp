#pragma once

#include <array>

typedef unsigned char byte;

short right_nibble(byte val);	// get the first number in a byte
short left_nibble(byte val);	// get the second number in a byte
short to_short(std::array<byte, 2> bytes);