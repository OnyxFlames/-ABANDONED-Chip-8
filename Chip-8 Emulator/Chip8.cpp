#include "Chip8.hpp"

#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>


Chip8::Chip8()
{
	window.create(sf::VideoMode(64 * pixel_size, 32 * pixel_size), emulation_title + "NO ROM"); 
	loadFont();
}

void Chip8::run()
{
		register unsigned frame_counter = 0;
		while (!ROM.eof() && ROM.is_open())
		{
			read();		// won't if paused
			input();
			update();	// won't if paused
			draw();
			update_frame_counter(frame_counter);
		}
		//update debug info one more time after ending rom
		debuginfo_updated = false;
		window.setTitle("[DEBUG]" + emulation_title);
		while (window.isOpen() && loaded_debug)
		{
			input();
			update();
			draw();
			update_frame_counter(frame_counter);
		}
}
bool Chip8::load_rom(const std::string ROM_location)
{
	ROM.open(ROM_location, std::ifstream::binary);
	if (!ROM.is_open())
	{
		emulation_title = emulation_title + "Error loading ROM";
		window.setTitle(emulation_title);
	}
	else
	{
		emulation_title = emulation_title + "\"" + ROM_location + "\"";
		window.setTitle(emulation_title);
	}
	return ROM.is_open();
}
void Chip8::pause(bool _pause)
{
	pause_emulation = _pause;
	if (pause_emulation)
		window.setTitle(emulation_title + "(PAUSED)");
	else
		window.setTitle(emulation_title);
}
void Chip8::input()
{
	if (!window.isOpen())
		ROM.seekg(std::ios::end);
	//TODO: Add the value of the i register to the bottom of the memory block page.
	const int text_spacing(56), vertical_limit(-3810);
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;
			case sf::Keyboard::F1:
				if (loaded_debug)
				{
					debug_flag = !debug_flag;
					winsize_updated = false;
				}
				break;
			case sf::Keyboard::Down:
				if (debug_flag)
					update_debugtext(text_spacing, vertical_limit, DOWN);
				break;
			case sf::Keyboard::Up:
				if (debug_flag)
					update_debugtext(text_spacing, vertical_limit, UP);
				break;
			case sf::Keyboard::Tab:
				if (debug_flag)
				{
					// switch between displaying memory or callstack
					show_memory = !show_memory;
					debuginfo_updated = false;
				}
				break;
			case sf::Keyboard::Tilde:
				pause_emulation = !pause_emulation;
				if (pause_emulation)
					window.setTitle(emulation_title + "(PAUSED)");
				else
					window.setTitle(emulation_title);
				break;
			default:
				std::cout << "UNSUPPORTED KEYCODE\n";
				break;
			}
		}
		/*
			TODO: Put code that loads keyboard input into the registers
		*/
	}
}
void Chip8::draw()
{
	if (debug_flag)
	{
		draw_debug();
	}
	else
	{
		window.clear(sf::Color::White);
		//for (auto _ : monitor)
			//for (auto __ : _)
				//window.draw(__);
		
				// You only wanna display the FPS if it's set to red(which means debug data was loaded)
		if (fps_text.getColor() == sf::Color::Red)
			window.draw(fps_text);
		window.display();
	}
}
void Chip8::update()
{
	if (debug_flag)
	{
		update_debug();
	}
	else
	{
		if (!winsize_updated)
		{
			window.setSize(sf::Vector2u(64 * pixel_size, 32 * pixel_size));
			winsize_updated = true;
		}
	}
	// always update debug info, but nothing else when paused.
	if (pause_emulation)
			return;

	// TODO: include check for sound and delay clocks here
}

void Chip8::read()
{
	// Skip attempt to read opcodes.. they aren't there
	if (!ROM.is_open())
	{
		if (loaded_debug)
			ROM.seekg(std::ios::end);
		else
			return;
	}

	if (pause_emulation)
		return;

	byte instruct_buff[2];

	instruct_buff[0] = ROM.get(); instruct_buff[1] = ROM.get();
	
	unsigned short jmp = 0x0000;
	unsigned short opcode = Onyx::to_ushort({ instruct_buff[0], instruct_buff[1] });
	switch (get_left(instruct_buff[0]))
	{
	case 0x00:
		switch (instruct_buff[1])
		{
		case 0xE0:
			call_stack.push_back("0x00E0 - Clear Screen");
			op_clear_screen();
			break;
		case 0xEE:
			call_stack.push_back("0x00EE - Return from Subroutine");
			if (stack.empty())
			{
				std::cout << "ATTEMPTED TO RETURN FROM UNKNOWN SUBROUTINE. Please fix.\n";
				std::exit(-1);
			}
			ROM.seekg(stack.top(), std::ios::beg);
			stack.pop();
			break;
		default:
			invalid_opcode(instruct_buff[0], instruct_buff[1]);
		break;
		}
		break;
	case 0x01:	// jump instruction
		call_stack.push_back("0x1NNN - Jump to NNN");
		jmp = (Onyx::to_ushort({ instruct_buff[0], instruct_buff[1] }) & 0xff);
		ROM.seekg(jmp, std::ios::beg);
		break;
	case 0x02:	// call subroutine
		call_stack.push_back("0x2NNN - Call subroutine at NNN");
		stack.push((unsigned short)ROM.tellg());
		jmp = (Onyx::to_ushort({ instruct_buff[0], instruct_buff[1] }) & 0xff);
		ROM.seekg(jmp, std::ios::beg);
		break;
	case 0x03:	// if VX == NN skip next instruction
		call_stack.push_back("0x3XNN - if VX == NN skip next instruction.");
		if (registers[get_right(instruct_buff[0])] == instruct_buff[1])
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
		}
		break;
	case 0x04:	// if VX != NN skip the next two
		call_stack.push_back("0x4XNN - if VX != NN skip next instruction.");
		if (registers[get_right(instruct_buff[0])] != instruct_buff[1])
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
		}
		break;
	case 0x05:	// if VX == VY skip the next two
		if (get_right(instruct_buff[1]) != 0x00)
		{
			invalid_opcode(instruct_buff[0], instruct_buff[1]);
			break;
		}
		call_stack.push_back("0x5XY0 - if VX == VY skip next instruction.");
		if (registers[get_right(instruct_buff[0])] == registers[get_left(instruct_buff[1])])
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
		}
		break;
	case 0x06:	// sets VX to NN
		call_stack.push_back("0x6XNN - set VX to NN");
		registers[get_right(instruct_buff[0])] = instruct_buff[1];
		break;
	case 0x07:	// adds VX to NN
		call_stack.push_back("0x7XNN - adds VX to NN");
		registers[get_right(instruct_buff[0])] += instruct_buff[1];
		break;
	case 0x08:
		switch (get_right(instruct_buff[1]))
		{
		case 0x00:	// set VX to the value of VY
			call_stack.push_back("0x8XY0 - set VX to the value of VY");
			registers[get_right(instruct_buff[0])] = registers[get_left(instruct_buff[1])];
			break;
		case 0x01:	// set VX to (VX | VY)
				call_stack.push_back("0x8XY1 - set VX to (VX | VY)");
				registers[get_right(instruct_buff[0])] |= registers[get_left(instruct_buff[1])];
			break;
		case 0x02:	// set VX to (VX & VY)
			call_stack.push_back("0x8XY2 - set VX to (VX & VY)");
			registers[get_right(instruct_buff[0])] &= registers[get_left(instruct_buff[1])];
			break;
		case 0x03:	// set VX to (VX ^ VY)
			call_stack.push_back("0x8XY3 - set VX to (VX ^ VY)");
			registers[get_right(instruct_buff[0])] ^= registers[get_left(instruct_buff[1])];
			break;
		case 0x04:
			call_stack.push_back("0x8XY4 - adds VY to VX (carry: VF = 1 || no carry: VF = 0)");
			if (registers[get_left(instruct_buff[1])] > (0xFF - registers[get_right(instruct_buff[0])]))
				registers[0x0F] = 0x01;
			else
				registers[0x0F] = 0x00;
			// VX += VY
			registers[get_left(instruct_buff[1])] += registers[get_right(instruct_buff[0])];
			break;
		case 0x05:
			call_stack.push_back("0x8XY5 - subtracts VY from VX (borrow: VF = 0 || no borrow: VF = 1)");
			if (registers[get_left(instruct_buff[1])] > registers[get_right(instruct_buff[0])])
				registers[0x0F] = 0x00;
			else
				registers[0x0F] = 0x01;
			// VX -= VY
			registers[get_right(instruct_buff[0])] -= registers[get_left(instruct_buff[1])];
			break;
		case 0x06:
			call_stack.push_back("0x8XY6 - shifts VX right by one. VF = value of LSB of VX before shift");
			registers[0x0F] = (get_right(instruct_buff[0]) & 0x01);
			registers[get_right(instruct_buff[0])] >>= 1;
			break;
		case 0x07:
			call_stack.push_back("0x8XY7 - VX = (VY - VX)(borrow: VF = 0 || no borrow: VF = 1)");
			if (registers[get_right(instruct_buff[0])] > registers[get_left(instruct_buff[1])])
				registers[0x0F] = 0x00;
			else
				registers[0x0F] = 0x01;
			// VX = VY - VX
			registers[get_right(instruct_buff[0])] = registers[get_left(instruct_buff[1])] - registers[get_right(instruct_buff[0])];
			break;
		case 0x0E:
			call_stack.push_back("0x8XYE - shifts VX left by one VF = MSB of VX before shift");
			registers[0x0F] = (get_right(instruct_buff[0]) & 0x08);
			registers[get_right(instruct_buff[0])] <<= 1;
			break;
		default:
			invalid_opcode(instruct_buff[0], instruct_buff[1]);
			break;
		}
		break;
	case 0x09:
		if (get_right(instruct_buff[1]) != 0x00)
		{
			invalid_opcode(instruct_buff[0], instruct_buff[1]);
			break;
		}
		call_stack.push_back("0x9XY0 - if VX != VY skip next instruction");
		if (get_right(instruct_buff[0]) != get_left(instruct_buff[1]))
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
		}
		break;
	case 0x0A:	// hexadecimal A - ANNN: set i to NNN
		call_stack.push_back("0xANNN - i = NNN");
		jmp += get_right(instruct_buff[0]);
		jmp <<= 8;
		jmp += instruct_buff[1];
		i = jmp;
		break;
	case 0x0B:	// hexadecimal B - BNNN: jump to NNN + V0
		call_stack.push_back("0xBNNN - jump to NNN + V0");
		jmp += get_right(instruct_buff[0]);
		jmp <<= 8;
		jmp += instruct_buff[1];
		ROM.seekg(jmp + registers[0x00], std::ios::beg);
		break;
	case 0x0C:	// hexadecimal C - sets VX to the result of (rand() & NN)
		call_stack.push_back("0xCXNN - VX = (rand() & NN)");
		registers[get_right(instruct_buff[0])] = (rand() % 255) & instruct_buff[1];
		break;
	case 0x0D:	// hexadecimal D - draw sprite at VX VY with a height of N and width of 8
		call_stack.push_back("0xDXYN - draw sprite at VX VY, N high[UNIMPLEMENTED]");
		break;
	case 0x0E:
		switch (instruct_buff[1])
		{
		case 0x9E:
			call_stack.push_back("0xEX9E - Skips next instruction if key in VX is pressed[UNIMPLEMENTED]");

			break;
		case 0xA1:
			call_stack.push_back("0xEXA1 - Skips next instruction if key in VX isn't pressed[UNIMPLEMENTED]");

			break;
		default:
			invalid_opcode(instruct_buff[0], instruct_buff[1]);
			break;
		}
		break;
	case 0x0F:
		switch (instruct_buff[1])
		{
		case 0x07:
			call_stack.push_back("0xFX07 - Sets VX to value of delay timer.[UNIMPLEMENTED]");

			break;
		case 0x0A:
			call_stack.push_back("0xFX0A - Key press is waited and stored in VX[UNIMPLEMENTED]");

			break;
		case 0x15:
			call_stack.push_back("0xFX15 - set delay timer to VX");
			delay_time = sf::seconds(registers[get_right(instruct_buff[0])]);
			break;
		case 0x18:
			call_stack.push_back("0xFX18 - set sound timer to VX");
			sound_time = sf::seconds(registers[get_right(instruct_buff[0])]);
			break;
		case 0x1E:
			call_stack.push_back("0xFX1E - adds VX to I");
			i += registers[get_right(instruct_buff[0])];
			break;
		case 0x29:
			call_stack.push_back("0xFX29 - sets i to the location of sprite for char in VX[UNIMPLEMENTED]");
			{
			const short font_origin = 0x0000;
			const short font_byte_size = 6;
			switch (registers[get_right(instruct_buff[0])])
			{
			case '0':
				i = font_origin;
				break;
			case '1':
				i = font_origin + font_byte_size * 1;
				break;
			case '2':
				i = font_origin + font_byte_size * 2;
				break;
			case '3':
				i = font_origin + font_byte_size * 3;
				break;
			case '4':
				i = font_origin + font_byte_size * 4;
				break;
			case '5':
				i = font_origin + font_byte_size * 5;
				break;
			case '6':
				i = font_origin + font_byte_size * 6;
				break;
			case '7':
				i = font_origin + font_byte_size * 7;
				break;
			case '8':
				i = font_origin + font_byte_size * 8;
				break;
			case '9':
				i = font_origin + font_byte_size * 9;
				break;
			case 'A':
				i = font_origin + font_byte_size * 10;
				break;
			case 'B':
				i = font_origin + font_byte_size * 11;
				break;
			case 'C':
				i = font_origin + font_byte_size * 12;
				break;
			case 'D':
				i = font_origin + font_byte_size * 13;
				break;
			case 'E':
				i = font_origin + font_byte_size * 14;
				break;
			case 'F':
				i = font_origin + font_byte_size * 15;
				break;
			default:
				i = font_origin;
				break;
			}
			}
			break;
		case 0x33:
			call_stack.push_back("0xFX33 - stores BCD of VX in memory. (look up for details)0[UNIMPLEMENTED]");
			break;
		case 0x55:
			call_stack.push_back("0xFX55 - Stores V0 to VX to memory starting at i");
			for (byte it = 0; it <= (get_right(instruct_buff[0])); it++)
				memory[i + it] = registers[it];
			break;
		case 0x65:
			call_stack.push_back("0xFX65 - Fills V0 to VX with values from memory starting at i");
			for (byte it = 0; it <= (get_right(instruct_buff[0])); it++)
				registers[it] = memory[i + it];
			break;
		default:
			if (instruct_buff[0] == 0xff && instruct_buff[1] == 0xff)
			{
				ROM.close();
				run();
				break;
			}
				invalid_opcode(instruct_buff[0], instruct_buff[1]);
			break;
		}
		break;
	default:
		invalid_opcode(instruct_buff[0], instruct_buff[1]);
		break;
	}
}

void Chip8::op_clear_screen()
{
	window.clear();
}
void Chip8::loadFont()
{						//font set size is 80 bytes.
	for (byte it = 0; it < 80; it++)
		memory[it] = fontset[it];
}
Chip8::~Chip8()
{
}
