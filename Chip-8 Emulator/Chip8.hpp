#pragma once

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <stack>
#include <string>
#include <vector>
#include <fstream>

#include "byte.hpp"

using Onyx::byte;
using Onyx::get_left;
using Onyx::get_right;

const int pixel_size = 8;	// treat each Chip-8 pixel as it is 8x8 real life pixels.
const unsigned mem_count_start = 0x00;

class Chip8
{
private:
	// Debug code
	bool loaded_debug = false;
	sf::Font debug_font;
	sf::Text fps_text;
	std::vector<sf::Text> debug_text;
	std::vector<sf::Text> address_text;
	std::vector<sf::Text> register_text;
	std::vector<std::string> call_stack;
	bool debug_flag = false;
	bool winsize_updated = false;
	bool debuginfo_updated = false;
	bool show_memory = true;	// if not memory, show the call stack.
	void load_debug_data();
	void update_frame_counter(unsigned &frame_counter);
	// ...
	std::string emulation_title = "Chip-8 - ";
	bool pause_emulation = false;
	std::ifstream ROM;
	sf::RenderWindow window;
	//Monitor monitor;
	bool input_wait = false;
	short pc = 0x0000;
	short i = 0x0000;
	byte registers[0x0010];
	byte memory[0x1000];
	std::stack<unsigned short> stack;

	sf::Clock delay_clock, sound_clock, perf_clock;
	sf::Time delay_time, sound_time;
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
	void set_debug(bool _debug);
	bool load_rom(const std::string ROM_location);
	void pause(bool _pause);
	~Chip8();
};

void invalid_opcode(byte &_byte, byte &__byte);