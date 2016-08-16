#include "Arguments.hpp"

#include <string>
#include <iostream>

extern bool dump_callstack;
extern bool pause_on_start;
extern bool load_debug_data;
extern std::string rom_name;

void handle_args(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-load--debug", argv[i]) == 0)
			load_debug_data = true;
		else if (strcmp("-pauseonstart", argv[i]) == 0)
			pause_on_start = true;
		else if (strcmp("-dump--callstack", argv[i]) == 0)
			dump_callstack = true;
		else
			rom_name = argv[i];	// note the rom name will get overwritten if multiple roms are passed. This method will use the last rom name given.
	}
}
void debug_prompt()
{
	std::cout << "[Debug] Press any key to exit.." << std::endl;
	getchar();
}