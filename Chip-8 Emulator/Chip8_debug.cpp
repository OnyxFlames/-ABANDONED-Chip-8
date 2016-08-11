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
	address_text.resize(_mem_size / _divisor);
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
		if (i < (_mem_size / _divisor))
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
			fps_text.setString("FPS: " + std::to_string(frame_counter));
			frame_counter = 0;
			perf_clock.restart();
		}
		else
			frame_counter++;
	}
	else
		;
}

void invalid_opcode(byte &_byte, byte &__byte)
{
	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)_byte << (int)__byte << " is not a valid opcode!\n" << std::dec;
}