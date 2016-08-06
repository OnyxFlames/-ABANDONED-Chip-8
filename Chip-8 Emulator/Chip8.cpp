#include "Chip8.hpp"

#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>

Chip8::Chip8()
{
	window.create(sf::VideoMode(64 * pixel_size, 32 * pixel_size), emulation_title + "NO ROM");
	init_all(monitor, pixel_size, sf::Color(100, 100, 100));
}

void Chip8::run()
{
		while (window.isOpen() && !ROM.eof())
		{
			read();		// won't if paused
			input();
			update();	// won't if paused
			draw();
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
						for (unsigned i = 0x00; i < 0x1000; i++)
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
						for (unsigned i = 0x00; i < 0x1000; i++)
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

				for (unsigned i = 0; i < 0x1000; i++)
				{
					if (i < 16)
						window.draw(register_text[i]);
					if (i < 0x1000 / 16)
						window.draw(address_text[i]);
					window.draw(debug_text[i]);
				}
			}
			else
			{
				// update callstack data
			}
			window.display();
			debuginfo_updated = true;
		}
	}
	else
	{
		window.clear();
		for (auto _ : monitor)
			for (auto __ : _)
				window.draw(__);
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
			for (unsigned i = 0x00; i < 0x1000; i++)
			{
				if (i < 16)
				{
					std::stringstream ss;
					ss << std::hex << "V" << i << ": " << std::setfill('0') << std::setw(3) << registers[i];
					register_text[i].setString(ss.str());
				}
				std::stringstream ss;
				ss << std::hex << std::setfill('0') << std::setw(3) << std::to_string(memory[i]);
				debug_text[i].setString(ss.str());
				ss.clear();
			}
			debuginfo_updated = true;
		}
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
		return;
	if (pause_emulation)
		return;
	byte instruct_buff[2];
	
	instruct_buff[0] = ROM.get(); instruct_buff[1] = ROM.get();

	debugger.print_bytes(instruct_buff[0], instruct_buff[1]);
	
	if (left_nibble(instruct_buff[0]) == 0)
	{
		if (instruct_buff[1] == 0xE0)	// opcode is 0x00E0
		{
			op_clear_screen();
		}
		else if (instruct_buff[1] == 0xEE)	// return from subroutine
		{
			if (stack.empty())
			{
				std::cout << "ATTEMPTED TO RETURN FROM UNKNOWN SUBROUTINE. Please fix.\n";
				std::exit(-1);
			}
			ROM.seekg(stack.top(), std::ios::beg);
			stack.pop();
		}
	}
	else if (left_nibble(instruct_buff[0]) == 1)	// jump instruction
	{
		short jmp = 0x0000;
		jmp += right_nibble(instruct_buff[0]);
		jmp <<= 8;
		jmp += instruct_buff[1];
		debugger.jump_msg(jmp);
		ROM.seekg(jmp, std::ios::beg);
	}
	else if (left_nibble(instruct_buff[0]) == 2)	// call subroutine
	{
		short jmp = 0x0000;
		stack.push((short)ROM.tellg());
		jmp += right_nibble(instruct_buff[0]);
		jmp <<= 8;
		jmp += instruct_buff[1];
		debugger.sub_msg(jmp);
		ROM.seekg(jmp, std::ios::beg);
	}
	else if (left_nibble(instruct_buff[0]) == 3)	// if VX == NN skip the next two
	{
		if (registers[right_nibble(instruct_buff[0])] == instruct_buff[1])
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
			debugger.print_skip(buff1, buff2);
		}
	}
	else if (left_nibble(instruct_buff[0]) == 4)	// if VX != NN skip the next two
	{
		if (registers[right_nibble(instruct_buff[0])] != instruct_buff[1])
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
			debugger.print_skip(buff1, buff2);
		}
	}
	else if (left_nibble(instruct_buff[0]) == 5)	// if VX == VY skip the next two
	{
		if (registers[right_nibble(instruct_buff[0])] == registers[left_nibble(instruct_buff[1])])
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
			debugger.print_skip(buff1, buff2);
		}
	}
	else if (left_nibble(instruct_buff[0]) == 6)	// sets VX to NN
	{
		registers[right_nibble(instruct_buff[0])] = instruct_buff[1];
	}
	else if (left_nibble(instruct_buff[0]) == 7)	// adds NN to VX
	{
		registers[right_nibble(instruct_buff[0])] += instruct_buff[1];
	}
	else if (left_nibble(instruct_buff[0]) == 8)
	{
		if (right_nibble(instruct_buff[1]) == 0)	// set VX to the value of VY
		{
			registers[right_nibble(instruct_buff[0])] = registers[left_nibble(instruct_buff[1])];
		}
		else if (right_nibble(instruct_buff[1]) == 1)
		{
			registers[right_nibble(instruct_buff[0])] |= registers[left_nibble(instruct_buff[1])];
		}
		else if (right_nibble(instruct_buff[1]) == 2)
		{
			registers[right_nibble(instruct_buff[0])] &= registers[left_nibble(instruct_buff[1])];
		}
		else if (right_nibble(instruct_buff[1]) == 3)
		{
			registers[right_nibble(instruct_buff[0])] ^= registers[left_nibble(instruct_buff[1])];
		}
		else if (right_nibble(instruct_buff[1]) == 4)
		{
			// TODO: add with carry opcode
		}
		else if (right_nibble(instruct_buff[1]) == 5)
		{
			// TODO: subtract with borrow opcode
		}
		else if (right_nibble(instruct_buff[1]) == 6)
		{
			// TODO: shift VX right by one -- VF equals last bit of VX before shift
		}
		else if (right_nibble(instruct_buff[1]) == 7)
		{
			// TODO: set VX to VY - VX -- VF is set to 0 when there is a borrow 1 when there isn't
		}
		else if (right_nibble(instruct_buff[1]) == 14)
		{
			// TODO: shifts VX left by one -- VF is set to the value of the first bit of VX before shift
		}
	}
	else if (left_nibble(instruct_buff[0]) == 9)
	{
		if (right_nibble(instruct_buff[0]) != left_nibble(instruct_buff[1]))
		{
			byte buff1, buff2;
			buff1 = ROM.get(); buff2 = ROM.get();
			debugger.print_skip(buff1, buff2);
		}
	}
	else if (left_nibble(instruct_buff[0]) == 10)	// hexadecimal A
	{
		short temp = 0x0000;
		temp += right_nibble(instruct_buff[0]);
		temp <<= 8;
		temp += instruct_buff[1];
		i = temp;
	}

}

void Chip8::op_clear_screen()
{
	color_pixels(monitor, sf::Color::Black);
}
Chip8::~Chip8()
{
}

void Chip8::set_debug(bool _debug)
{
	loaded_debug = _debug;
	if (loaded_debug)
		load_debug_data();
}

void Chip8::load_debug_data()
{
	// TODO: Add code for call stack and stack data (as a debug toggle)
	sf::Color debug_text_color = sf::Color::White;
	// Debug code
	debug_font.loadFromFile("../DEBUG_RESOURCES/debug_font.ttf");
	debug_text.resize(0x1000);
	address_text.resize(0x1000 / 16);
	register_text.resize(16);
	for (unsigned i = 0x00; i < 0x1000; i++)
	{
		const float interval(24), vertical_spacing(3), width_limit(60), start_pos(120);
		static float x(-interval + start_pos), y(0);
		x += interval;
		if (x > (width_limit * pixel_size))
		{
			x = start_pos;
			y += interval - (interval / vertical_spacing);
			debug_text[i].setPosition(x, y);
		}
		else
		{
			debug_text[i].setPosition(x, y);
		}
		debug_text[i].setString("000");
		debug_text[i].setFont(debug_font);
		debug_text[i].setColor(debug_text_color);
		debug_text[i].setCharacterSize(12);
		if (i < (0x1000 / 16))
		{
			std::stringstream ss;
			ss << std::hex << "0x" << std::setfill('0') << std::setw(3) << (i * 16);
			address_text[i].setString(ss.str() + ":");
			address_text[i].setPosition(64.0f, (interval - (interval / vertical_spacing)) * i);
			address_text[i].setFont(debug_font);
			address_text[i].setColor(debug_text_color);
			address_text[i].setCharacterSize(12);
			ss.clear();
		}
		if (i < 16)
		{
			register_text[i].setFont(debug_font);
			register_text[i].setColor(debug_text_color);
			register_text[i].setCharacterSize(12);
			// Set register names and placement
			switch (i)
			{
			case 0: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 1: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 2: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 3: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 4: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 5: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 6: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 7: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 8: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 9: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 10: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 11: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 12: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 13: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 14: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			case 15: register_text[i].setPosition(2.f, (interval - (interval / vertical_spacing)) * i); break;
			}
		}
	}
	debugger.set_write(false);
	// ...
}

void Chip8::debug_print_registers()
{
	for (int i = 0; i < 16; i++)
		std::cout << std::hex << (int)registers[i] << " ";
	std::cout << "\n---\n";
}