#include "Chip8.hpp"

#include <sstream>
#include <iomanip>

void Chip8::set_debug(bool _debug)
{
	loaded_debug = _debug;
	if (loaded_debug)
		load_debug_data();
}

void Chip8::load_debug_data()
{
	const int _mem_size = 0x1000;
	const int _divisor = 0x10;
	const sf::Color debug_text_color = sf::Color::White;
	const sf::Color debug_addrtest_color = sf::Color(200, 200, 200);
	const sf::Color fps_text_color = sf::Color::Red;
	// Debug code
	debug_font.loadFromFile("../DEBUG_RESOURCES/debug_font.ttf");
	debug_text.resize(_mem_size);
	address_text.resize(((_mem_size / _divisor) + 1));
	register_text.resize(_divisor);
	fps_text.setFont(debug_font);
	fps_text.setColor(fps_text_color);
	fps_text.setCharacterSize(12);
	fps_text.setString("Waiting...");
	for (unsigned i = mem_count_start; i < _mem_size; i++)
	{
		const float interval(24), vertical_spacing(3), width_limit(60), start_pos(120);
		static float x(-interval + start_pos), y(0);
		x += interval;
		if (x >(width_limit * pixel_size))
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
		if (i < ((_mem_size / _divisor) + 1))
		{
			std::stringstream ss;
			if (i < (_mem_size / _divisor))
			{
				ss << std::hex << "0x" << std::setfill('0') << std::setw(3) << (i * 16);
				address_text[i].setString(ss.str() + ":");
				address_text[i].setPosition(64.0f, (interval - (interval / vertical_spacing)) * i);
				address_text[i].setFont(debug_font);
				address_text[i].setColor(debug_text_color);
				address_text[i].setCharacterSize(12);
				ss.clear();
			}
			else
			{
				ss << std::hex << "Register i: " << std::setfill('0') << std::setw(3) << this->i;
				address_text[i].setString(ss.str());
				address_text[i].setPosition(64.0f, (interval - (interval / vertical_spacing)) * i);
				address_text[i].setFont(debug_font);
				address_text[i].setColor(debug_text_color);
				address_text[i].setCharacterSize(12);
			}
		}
		if (i < _divisor)
		{
			register_text[i].setFont(debug_font);
			register_text[i].setColor(debug_addrtest_color);
			register_text[i].setCharacterSize(12);
			// Set register names and placement
			switch (i)
			{
			case 0: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 1: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 2: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 3: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 4: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 5: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 6: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 7: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 8: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 9: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 10: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 11: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 12: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 13: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 14: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			case 15: register_text[i].setPosition(1.5f, (interval - (interval / vertical_spacing)) * i); break;
			}
		}
	}
	// ...
}

void Chip8::update_frame_counter(unsigned &frame_counter)
{
	if (loaded_debug)
	{
		if (perf_clock.getElapsedTime() >= sf::milliseconds(1000))
		{
			fps_text.setString("FPS: " + std::to_string(frame_counter) + " pc: " + std::to_string(pc));
			frame_counter = 0;
			perf_clock.restart();
		}
		else
			frame_counter++;
	}
	else
		;
}

void Chip8::update_debug()
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
				if (i < ((0x1000 / 16) + 1))
				{
					std::stringstream ss;
					if (i < (0x1000 / 16))
					{
						ss << std::hex << "0x" << std::setfill('0') << std::setw(3) << (i * 16);
						address_text[i].setString(ss.str() + ":");
						ss.clear();
					}
					else
					{
						ss << std::hex << "Register i: " << std::setfill('0') << std::setw(4) << this->i;
						address_text[i].setString(ss.str());
						ss.clear();
					}
				}
				std::stringstream ss;
				ss << std::hex << std::setfill('0') << std::setw(2) << (int)memory[i];
				debug_text[i].setString(ss.str());
				if (strcmp(debug_text[i].getString().toAnsiString().c_str(), "00") != 0)
					debug_text[i].setColor(sf::Color::Red);
				else
					debug_text[i].setColor(sf::Color::White);
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

void Chip8::draw_debug()
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
				if (i < ((0x1000 / 16) + 1))
					window.draw(address_text[i]);
				window.draw(debug_text[i]);
			}
		}
		else if (!show_memory)
		{
			// to lessen the load on the Chip8 class, we will reuse the address_text vector
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

void Chip8::update_debugtext(const int &_spacing, const int &_limit, const char _direction)
{
	if (_direction == DOWN)
	{
		if (debug_text[0].getPosition().y > _limit || address_text[0].getPosition().y > _limit)
		{
			for (unsigned i = mem_count_start; i < 0x1000; i++)
			{
				if (i < ((0x1000 / 16) + 1))
					address_text[i].setPosition(address_text[i].getPosition().x, address_text[i].getPosition().y - _spacing);

				debug_text[i].setPosition(debug_text[i].getPosition().x, debug_text[i].getPosition().y - _spacing);
			}
		}
		else
			;// don't move it
	}
	else if (_direction == UP)
	{
		if (debug_text[0].getPosition().y < 0 || address_text[0].getPosition().y < 0)
		{
			for (unsigned i = mem_count_start; i < 0x1000; i++)
			{
				if (i < ((0x1000 / 16) + 1))
					address_text[i].setPosition(address_text[i].getPosition().x, address_text[i].getPosition().y + _spacing);

				debug_text[i].setPosition(debug_text[i].getPosition().x, debug_text[i].getPosition().y + _spacing);
			}
		}
		else
			;// don't move it
	}
}

void invalid_opcode(byte &_byte, byte &__byte)
{
	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)_byte << std::setfill('0') << std::setw(2) << (int)__byte << " is not a valid opcode!\n" << std::dec;
}

void Chip8::dump_callstack(bool _dump)
{
	if (_dump)
	{
		std::ofstream output("CALLSTACK");
		if (!output.is_open())
		{
			std::cout << "ERROR CREATING CALLSTACK DUMP FILE\n";
			return;
		}
		for (auto it : call_stack)
			output << it << "\n";
	}
}

byte sfkeytohexkey(short sfkey)
{
	switch (sfkey)
	{
	case sf::Keyboard::Num1:
		return 1;
		break;
	case sf::Keyboard::Num2:
		return 2;
		break;
	case sf::Keyboard::Num3:
		return 3;
		break;
	case sf::Keyboard::Num4:
		return 12;
		break;
	case sf::Keyboard::Q:
		return 4;
		break;
	case sf::Keyboard::W:
		return 5;
		break;
	case sf::Keyboard::E:
		return 6;
		break;
	case sf::Keyboard::R:
		return 13;
		break;
	case sf::Keyboard::A:
		return 7;
		break;
	case sf::Keyboard::S:
		return 8;
		break;
	case sf::Keyboard::D:
		return 9;
		break;
	case sf::Keyboard::F:
		return 14;
		break;
	case sf::Keyboard::Z:
		return 10;
		break;
	case sf::Keyboard::X:
		return 0;
		break;
	case sf::Keyboard::C:
		return 11;
		break;
	case sf::Keyboard::V:
		return 15;
		break;
	default:
		return -1;
	}
}
sf::Keyboard::Key hexkeytosfkey(byte hexkey)
{
	switch (hexkey)
	{
	case 1:
		return sf::Keyboard::Num1;
		break;
	case 2:
		return sf::Keyboard::Num2;
		break;
	case 3:
		return sf::Keyboard::Num3;
		break;
	case 12:
		return sf::Keyboard::Num4;
		break;
	case 4:
		return sf::Keyboard::Q;
		break;
	case 5:
		return sf::Keyboard::W;
		break;
	case 6:
		return sf::Keyboard::E;
		break;
	case 13:
		return sf::Keyboard::R;
		break;
	case 7:
		return sf::Keyboard::A;
		break;
	case 8:
		return sf::Keyboard::S;
		break;
	case 9:
		return sf::Keyboard::D;
		break;
	case 14:
		return sf::Keyboard::F;
		break;
	case 10:
		return sf::Keyboard::Z;
		break;
	case 0:
		return sf::Keyboard::X;
		break;
	case 11:
		return sf::Keyboard::C;
		break;
	case 15: 
		return sf::Keyboard::V;
		break;
	default:
		return sf::Keyboard::Escape;
	}
}

byte fontset[80] =	// from http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};