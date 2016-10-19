# Goal
The goal for this project is to have a Chip-8 emulator capable of fully emulating Chip-8 games and programs.

# Set up
This project currently uses SFML 2.2 for graphics and event handling, and is compiled using Visual Studio 2013 toolset.
To get the project up and running open the Chip-8 Emulator.sln file in Visual Studio 2013.
The default configuration has SFML 2.2 installed at **C:/SFML/SFML-2.2-vc12**. If you wish to change that, see the [SFML installation tutorial](http://www.sfml-dev.org/tutorials/2.2/start-vc.php) to figure out what you need to change.

# Usage
```Chip8 <args>```

Valid arguments are
* -load--debug
* -pauseonstart
* -dump--callstack
* Or a valid Chip8 ROM file.

# Debugging
The emulator comes with a few tools used for debugging development.

To turn on the debug display press F1 and press it again to toggle it off; you can alternate between memory and the callstack by pressing TAB while in debug mode.
Up and down arrows to scroll through memory block.

The debugging features need a system font to display the data, which is not supplied on the repo (copyright concerns).

The font used on the my local repository is Consolas, but most clean monotype fonts should work.

To set up a debug font, just open up the DEBUG\_RESOURCES folder and place your font face in there, and name it "debug\_font.ttf".
