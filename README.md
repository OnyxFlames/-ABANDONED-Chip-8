# Goal
The goal for this project is to have a Chip-8 emulator capable of fully emulating Chip-8 games and programs.
# Usage
```Chip8 <ROM>```

Or

```Chip8 <args../ROM>```

# Debugging
The emulator comes with a few tools used for debugging development.

To turn on the debug display press F1 and press it again to toggle it off; you can alternate between memory and the callstack by pressing TAB while in debug mode.
Up and down arrows to scroll through memory block.

The debugging features need a system font to display the data, which is not supplied on the repo (copyright concerns).

The font used on the my local repository is Consolas, but most clean monotype fonts should work.

To set up a debug font, just open up the DEBUG\_RESOURCES folder and place your font face in there, and name it "debug\_font.ttf".
