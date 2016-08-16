#include "Pixel.hpp"


Pixel::Pixel()
{

}

Pixel::Pixel(bool _is_set) : set(_is_set)
{
	pixel.setFillColor(sf::Color::White);
}

void Pixel::set_pixel(bool _set)
{
	set = _set;
}

bool Pixel::is_set()
{
	return set;
}

sf::RectangleShape& Pixel::get_pixel()
{
	return pixel;
}

Pixel::~Pixel()
{
}
