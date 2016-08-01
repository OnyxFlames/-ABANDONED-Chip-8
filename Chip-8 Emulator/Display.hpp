#pragma once

#include <SFML/Graphics.hpp>

#include <array>

// constants and typedefs
const int pixel_size = 8;
typedef std::array<std::array<sf::RectangleShape, 32>, 64> Monitor;


// helper function declarations
void init_all(Monitor &_screen, const int _pixel_size, sf::Color _color);
void initialize_pixels(Monitor &_screen, const int _pixel_size);
void space_pixels(Monitor &_screen, const int _pixel_size);
void color_pixels(Monitor &_screen, sf::Color _color);