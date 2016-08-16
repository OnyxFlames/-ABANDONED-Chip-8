#pragma once
#include "SFML/Graphics.hpp"
class Pixel
{
private:
	bool set = true;
	sf::RectangleShape pixel;
public:
	Pixel();
	Pixel(bool _is_set);
	void set_pixel(bool _set);
	bool is_set();
	sf::RectangleShape& get_pixel();
	~Pixel();
};

