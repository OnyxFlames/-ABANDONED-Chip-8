#pragma once

#include <SFML/Main.hpp>

#include <stack>
#include <string>
#include <fstream>

#include "byte.hpp"
#include "Display.hpp"

class Chip8
{
private:
	// program counter is ROM.tellg()
	std::ifstream ROM;
	sf::RenderWindow window;
	Monitor monitor;

	short i = 0x0000;
	byte registers[0x10];
	byte memory[0x1000];
	std::stack<short> stack;

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

