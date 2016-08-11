#include "Chip8.hpp"

#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>


Chip8::Chip8()
{
	window.create(sf::VideoMode(64 * pixel_size, 32 * pixel_size), emulation_title + "NO ROM");
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
	ROM.open(ROM_location);
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
	//TODO: find a sweet spot for the vertical limit so it ends on the bottom of the window instead of slightly above bottom
	const int text_spacing = 56, vertical_limit(-3810);
	while (window.pollEvent(event))
	{
		// TODO: Make closing the emulator cleaner once more progress has been made.
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::F1:
				if (loaded_debug)
				{
					debug_flag = !debug_flag;
					winsize_updated = false;
				}
				break;
			case sf::Keyboard::Down:
				if (debug_flag)
				{
					if (debug_text[0].getPosition().y > vertical_limit || address_text[0].getPosition().y > vertical_limit)
					{
						for (unsigned i = mem_count_start; i < 0x1000; i++)
						{
							if (i < 0x1000 / 16)
								address_text[i].setPosition(address_text[i].getPosition().x, address_text[i].getPosition().y - text_spacing);

							debug_text[i].setPosition(debug_text[i].getPosition().x, debug_text[i].getPosition().y - text_spacing);
						}
					}
					else
						;// don't move it
				}
				break;
			case sf::Keyboard::Up:
				if (debug_flag)
				{
					if (debug_text[0].getPosition().y < 0 || address_text[0].getPosition().y < 0)
					{
						for (unsigned i = mem_count_start; i < 0x1000; i++)
						{
							if (i < 0x1000 / 16)
								address_text[i].setPosition(address_text[i].getPosition().x, address_text[i].getPosition().y + text_spacing);

							debug_text[i].setPosition(debug_text[i].getPosition().x, debug_text[i].getPosition().y + text_spacing);
						}
					}
					else
						;// don't move it
				}
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
				// not a supported key code.
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
		if (debuginfo_updated)
		{
			window.clear();
			if (show_memory)
			{
				for (unsigned i = mem_count_start; i < 0x1000; i++)
				{
					if (i < 16)
						window.draw(register_text[i]);
					if (i < 0x1000 / 16)
						window.draw(address_text[i]);
					window.draw(debug_text[i]);
				}
			}
			else if (!show_memory)
			{
				// to lessen the load on the Chip8 class, we will reuse the debug_text vector
				for (unsigned i = mem_count_start; i < 0x1000; i++)
				{
					if (i < (0x1000 / 16))
						window.draw(address_text[i]);
				}
			}
			window.display();
			debuginfo_updated = true;
		}
	}
	else
	{
		window.clear(sf::Color::White);
		//for (auto _ : monitor)
			//for (auto __ : _)
				//window.draw(__);
		// You only wanna display the FPS if it's set to read(which means debug data was loaded)
		if (fps_text.getColor() == sf::Color::Red)
			window.draw(fps_text);
		window.display();
	}
}
void Chip8::update()
{
	if (debug_flag)
	{
		if (!winsize_updated)
		{
			window.setSize(sf::Vector2u(64 * pixel_size * 2, 32 * pixel_size * 2));
			winsize_updated = true;
		}
		if (!debuginfo_updated)
		{
			if (show_memory)
			{
					// update address text quick
					for (unsigned i = mem_count_start; i < 0x1000; i++)
					{
						if (i < 16)
						{
							std::stringstream ss;
							ss << std::hex << "V" << i << ": 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)registers[i];
							register_text[i].setString(ss.str());
						}
						if (i < (0x1000 / 16))
						{
							std::stringstream ss;
							ss << std::hex << "0x" << std::setfill('0') << std::setw(3) << (i * 16);
							address_text[i].setString(ss.str() + ":");
							ss.clear();
						}
						std::stringstream ss;
						ss << std::hex << std::setfill('0') << std::setw(3) << std::to_string(memory[i]);
						debug_text[i].setString(ss.str());
						ss.clear();
					}
				}
				else if (!show_memory)
				{

					for (unsigned i = mem_count_start; i < 0x1000; i++)
					{
						if (i < (0x1000 / 16))
						{
							if (call_stack.size() > i)
							{
								address_text[i].setString(call_stack[i]);
							}
							else
								address_text[i].setString("");
						}
					}
				}
		}
			debuginfo_updated = true;
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
	
	short jmp = 0x0000;

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
		jmp += get_right(instruct_buff[0]);
		jmp <<= 8;
		jmp += instruct_buff[1];
		ROM.seekg(jmp, std::ios::beg);
		break;
	case 0x02:	// call subroutine
		call_stack.push_back("0x2NNN - Call subroutine at NNN");
		stack.push((short)ROM.tellg());
		jmp += get_right(instruct_buff[0]);
		jmp <<= 8;
		jmp += instruct_buff[1];
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
			call_stack.push_back("0x8XY4 - adds VY to VX (carry: VF = 1 || no carry: VF = 0)[UNIMPLEMENTED]");
			// TODO: add with carry opcode
			break;
		case 0x05:
			call_stack.push_back("0x8XY5 - subtracts VY from VX (borrow: VF = 0 || no borrow: VF = 1)[UNIMPLEMENTED]");
			// TODO: subtract with borrow opcode
			break;
		case 0x06:
			call_stack.push_back("0x8XY6 - shifts VX right by one. VF = value of LSB of VX before shift[UNIMPLEMENTED]");
			// TODO: shift VX right by one -- VF equals last bit of VX before shift
			break;
		case 0x07:
			call_stack.push_back("0x8XY7 - VX = (VY - VX)(borrow: VF = 0 || no borrow: VF = 1)[UNIMPLEMENTED]");
			// TODO: set VX to VY - VX -- VF is set to 0 when there is a borrow 1 when there isn't
			break;
		case 0x0E:
			call_stack.push_back("0x8XYE - shifts VX left by one VF = MSB of VX before shift[UNIMPLEMENTED]");
			// TODO: shifts VX left by one -- VF is set to the value of the first bit of VX before shift
			break;
		default:
			invalid_opcode(instruct_buff[0], instruct_buff[1]);
			break;
		}
		break;
	case 0x09:
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
		call_stack.push_back("0xDXYN - draw sprite at VX VY, N high");
		registers[get_right(instruct_buff[0])] = (rand() % 255) & instruct_buff[1];
		break;


	case 0x0f:
		switch (instruct_buff[1])
		{
		case 0x07:

			break;
		case 0x0A:

			break;
		case 0x15:

			break;
		case 0x18:

			break;
		case 0x1E:

			break;
		case 0x29:

			break;
		case 0x33:

			break;
		case 0x55:

			break;
		case 0x65:

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

	default:
		invalid_opcode(instruct_buff[0], instruct_buff[1]);
		break;
	}
}

void Chip8::op_clear_screen()
{
	window.clear();
}
Chip8::~Chip8()
{
}

