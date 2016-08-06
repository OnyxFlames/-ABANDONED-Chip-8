#pragma once

#include <SFML/Main.hpp>

#include <stack>
#include <string>
#include <fstream>

#include "byte.hpp"
#include "Display.hpp"
#include "Debugger.hpp"
class Chip8
{
private:
	// program counter is ROM.tellg()
	// Debug code
	sf::Font debug_font;
	std::vector<sf::Text> debug_text;
	std::vector<sf::Text> address_text;
	bool debug_flag = false;
	bool winsize_updated = false;
	bool debuginfo_updated = false;
	Debugger debugger;
	// ...
	std::string emulation_title = "Chip-8 - ";
	bool pause_emulation = false;
	std::ifstream ROM;
	sf::RenderWindow window;
	Monitor monitor;

	short i = 0x0000;
	byte registers[0x10];
	byte memory[0x1000];
	std::stack<unsigned short> stack;

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
	void debug_print_registers();
	~Chip8();
};

