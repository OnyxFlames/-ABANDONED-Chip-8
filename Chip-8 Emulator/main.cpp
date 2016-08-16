#include <cassert>
#include <iostream>
#include <string>

#include "Chip8.hpp"
#include "Arguments.hpp"

bool dump_callstack = false;
bool pause_on_start = false;
bool load_debug_data = false;
std::string rom_name = "";
int main(int argc, char* argv[])
{

	assert(sizeof(short) * 8 == 16);

	if (argc < 2) 
	{
		std::cout << "Usage:\tChip8 <ROM> OR\n\tChip8 <args../ROM>\n";
		return -1;
	}
	else
	{
		handle_args(argc, argv);
	}

	Chip8 chip8;
	if (rom_name != "")
		chip8.load_rom(rom_name);
	
	chip8.set_debug(load_debug_data);	// load debug info (heavy function)
	chip8.pause(pause_on_start);
	chip8.run();
	chip8.dump_callstack(dump_callstack);
	return 0;
}