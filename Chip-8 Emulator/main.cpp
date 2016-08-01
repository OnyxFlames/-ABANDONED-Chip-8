#include <SFML/Graphics.hpp>
#include <array>

#include <iostream>

#include "Display.hpp"
#include "Chip8.hpp"

int main(int argc, char* argv[])
{
	Chip8 chip8;
	chip8.run();
	

	return 0;
}