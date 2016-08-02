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