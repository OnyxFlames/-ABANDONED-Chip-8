#include "Debugger.hpp"


Debugger::Debugger() 
{
}
Debugger::Debugger(bool _is_writing) : is_writing(_is_writing)
{
}

void Debugger::print_bytes(byte byte1, byte byte2, std::ostream &stream)
{
	if(is_writing)
		stream << "Buffer: " << std::hex << std::setfill('0') << std::setw(2) << (int)byte1 << std::setfill('0') << std::setw(2) << (int)byte2 << "\n" << std::dec;
}

void Debugger::print_skip(byte byte1, byte byte2, std::ostream &stream)
{
	if(is_writing)
		stream << "Skipping: " << std::hex << std::setfill('0') << std::setw(2) << (int)byte1 << std::setfill('0') << std::setw(2) << (int)byte2 << "\n" << std::dec;
}

void Debugger::jump_msg(short location, std::ostream &stream)
{
	if(is_writing)
		stream << "Jumping to: " << std::hex << std::setfill('0') << std::setw(2) << location << "\n" << std::dec;
}

void Debugger::sub_msg(short location, std::ostream &stream)
{
	if (is_writing)
		stream << "Calling sub: " << std::hex << std::setfill('0') << std::setw(2) << location << "\n" << std::dec;
}

void Debugger::set_write(bool _is_writing)
{
	is_writing = _is_writing;
}

Debugger::~Debugger()
{
}
