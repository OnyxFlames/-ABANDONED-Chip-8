#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>
#include <iostream>

#include "Display.hpp"
#include "Chip8.hpp"

#include "byte.hpp"


int main(int argc, char* argv[])
{

	assert(sizeof(short) * 8 == 16);

	//std::cout << left_nibble(0x12) << "\n";
	Chip8 chip8;
	chip8.load_rom("../ROMS/BRIX");
	chip8.run();

	return 0;
}