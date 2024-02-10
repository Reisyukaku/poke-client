# poke-client

Very WIP tool for debugging and reversing pokemon games. Based on exlaunch.

Designed to be ran on real hardware, but should work on emu as well.

## Features
- Hex viewer
- Lua VM (runs a lua script off the SD card)
- Lua print hooked to output to info panel
- Write logs to SD card
- Mouse and keyboard support

## Screenshot
![sv](https://i.imgur.com/XxItAzp.jpeg)

## Building
Run `make -j8`

Change the TitleID in the makefile and make sure the appropriate named json exists as well.

## Credits

- [exlaunch](https://github.com/shadowninja108/exlaunch/)
