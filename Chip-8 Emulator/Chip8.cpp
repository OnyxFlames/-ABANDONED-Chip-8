#include "Chip8.hpp"



Chip8::Chip8()
{
	window.create(sf::VideoMode(64 * pixel_size, 32 * pixel_size), "Chip-8 - NO ROM");
	init_all(monitor, pixel_size, sf::Color(127, 127, 127));
}

void Chip8::run()
{
		while (window.isOpen() && !ROM.eof())
		{
			read();
			input();
			update();
			draw();
		}
}

void Chip8::input()
{
	while (window.pollEvent(event))
	{
		// TODO: Make closing the emulator cleaner once more progress has been made.
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		/*
			TODO: Put code that loads keyboard input into the registers
		*/
	}
}
void Chip8::draw()
{
	window.clear();
	for (auto _ : monitor)
		for (auto __ : _)
			window.draw(__);
	window.display();
}
void Chip8::update()
{
	// TODO: include check for sound and delay clocks here
}

void Chip8::read()
{

}

void Chip8::op_clear_screen()
{
	color_pixels(monitor, sf::Color::Black);
}
Chip8::~Chip8()
{
}
