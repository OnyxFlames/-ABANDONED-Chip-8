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

	assert((sizeof(short) * 8) == 16);

	if (argc < 2) 
	{
		std::cout << "Usage:\t" << argv[0] << " <args>";
		return -1;
	}
	else
	{
		handle_args(argc, argv);
	}
	std::cout << "Chip8 Object Size: " << sizeof(Chip8) << " bytes.\n";
	Chip8 chip8;
	if (rom_name != "")
		chip8.load_rom(rom_name);
	
	chip8.set_debug(load_debug_data);	// load debug info (heavy function)
	chip8.pause(pause_on_start);
	chip8.run();
	chip8.dump_callstack(dump_callstack);
	return 0;
}