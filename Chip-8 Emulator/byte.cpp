#include "byte.hpp"


short right_nibble(byte val)
{
	short ret;
	val <<= 4;
	ret = (val >>= 4);
	return ret;
}
short left_nibble(byte val)
{
	short ret = (val >>= 4);
	return ret;
}

short to_short(std::array<byte, 2> bytes)
{
	short ret = bytes[0];
	ret <<= 8;
	ret += bytes[1];
	return ret;
}