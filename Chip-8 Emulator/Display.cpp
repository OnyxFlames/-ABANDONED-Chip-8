#include "Display.hpp"

void init_all(Monitor &_screen, const int _pixel_size, sf::Color _color)
{
	initialize_pixels(_screen, _pixel_size);
	space_pixels(_screen, _pixel_size);
	color_pixels(_screen, _color);
}

void initialize_pixels(Monitor &_screen, const int _pixel_size)
{
	for (int i = 0; i < _screen.size(); i++)
	{
		for (int j = 0; j < _screen[i].size(); j++)
		{
			_screen[i][j].setSize(sf::Vector2f(_pixel_size, _pixel_size));
		}
	}
}

void space_pixels(Monitor &_screen, const int _pixel_size)
{
	for (int i = 0; i < _screen.size(); i++)
	{
		for (int j = 0; j < _screen[i].size(); j++)
		{
			_screen[i][j].setPosition(i * _pixel_size, j * _pixel_size);
		}
	}
}

void color_pixels(Monitor &_screen, sf::Color _color)
{
	for (int i = 0; i < _screen.size(); i++)
	{
		for (int j = 0; j < _screen[i].size(); j++)
		{
			_screen[i][j].setFillColor(_color);
		}
	}
}