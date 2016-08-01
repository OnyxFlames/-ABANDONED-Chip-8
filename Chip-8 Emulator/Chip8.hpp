#pragma once

#include <SFML/Main.hpp>

#include <string>
#include <fstream>

#include "byte.hpp"
#include "Display.hpp"

class Chip8
{
private:
	std::ifstream ROM;
	sf::RenderWindow window;
	Monitor monitor;

	byte registers[0x0F];
	byte memory[0x1000];

	sf::Clock delay, sound;
	sf::Event event;
	// core functions
	void read();
	void input();
	void update();
	void draw();
	// ...
	// opcode functions
	void op_clear_screen();
	// ...
public:
	Chip8();
	void run();
	bool load_rom(const std::string ROM_location);
	~Chip8();
};

