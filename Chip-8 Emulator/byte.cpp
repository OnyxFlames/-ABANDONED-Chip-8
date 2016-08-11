#include "byte.hpp"

int16_t Onyx::to_short(std::array<byte, 2> _bytes)
{
	int16_t ret = _bytes[0];
	ret <<= 8;
	ret += _bytes[1];
	return ret;
}
uint16_t Onyx::to_ushort(std::array<byte, 2> _bytes)
{
	uint16_t ret = _bytes[0];
	ret <<= 8;
	ret += _bytes[1];
	return ret;
}
int32_t Onyx::to_int(std::array<byte, 4> _bytes)
{
	int32_t ret = _bytes[0];
	for (unsigned it = 1; it < 4; it++)
	{
		ret <<= 8;
		ret += _bytes[it];
	}
	return ret;
}
uint32_t Onyx::to_uint(std::array<byte, 4> _bytes)
{
	uint32_t ret = _bytes[0];
	for (unsigned it = 1; it < 4; it++)
	{
		ret <<= 8;
		ret += _bytes[it];
	}
	return ret;
}
int64_t Onyx::to_long(std::array<byte, 8> _bytes)
{
	int64_t ret = _bytes[0];
	for (unsigned it = 1; it < 8; it++)
	{
		ret <<= 8;
		ret += _bytes[it];
	}
	return ret;
}
uint64_t Onyx::to_ulong(std::array<byte, 8> _bytes)
{
	uint64_t ret = _bytes[0];
	for (unsigned it = 1; it < 8; it++)
	{
		ret <<= 8;
		ret += _bytes[it];
	}
	return ret;
}
float Onyx::to_float(std::array<byte, 4> _bytes)
{
	int32_t temp = _bytes[0];
	float ret = 0.00;
	for (unsigned it = 1; it < 4; it++)
	{
		temp <<= 8;
		temp += _bytes[it];
	}
	ret = static_cast<float>(temp);
	return ret;
}
double Onyx::to_double(std::array<byte, 8> _bytes)
{
	int64_t temp = _bytes[0];
	double ret = 0.00;
	for (unsigned it = 1; it < 8; it++)
	{
		temp <<= 8;
		temp += _bytes[it];
	}
	ret = static_cast<double>(temp);
	return ret;
}
Onyx::byte Onyx::get_left(Onyx::byte _byte)
{
	return ((Onyx::byte)(_byte & 0xF0) >> 4);
}
Onyx::byte Onyx::get_right(Onyx::byte _byte)
{
	return ((Onyx::byte)(_byte & 0x0F));
}
Onyx::byte Onyx::get_byte(std::istream &stream)
{
	byte ret = stream.get();
	while (isspace(ret))
		ret = stream.get();
	return ret;
}