#pragma once

#include "byte.hpp"

#include <iostream>
#include <iomanip>


class Debugger
{
private:
	bool is_writing = false;
public:
	Debugger();
	Debugger(bool _is_writing);
	void print_bytes(byte byte1, byte byte2, std::ostream &stream = std::cout);
	void print_skip(byte byte1, byte byte2, std::ostream &stream = std::cout);
	void jump_msg(short location, std::ostream &stream = std::cout);
	void sub_msg(short location, std::ostream &stream = std::cout);

	void set_write(bool _is_writing);
	~Debugger();
};

