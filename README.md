# poke-client

Tool for debugging and reversing pokemon games. Based on exlaunch.

Designed to be ran on real hardware.

## Features
- Hex editor
- Lua VM (runs a lua script off the SD card)
- Lua print hooked to output to info panel
- Write logs to SD card
- Mouse and keyboard support

## Screenshot
![sv](https://i.imgur.com/xjGxu0a.jpeg)

## Building
Install Docker and make sure its daemon is running and accessible to your user, then run:

```sh
make -j8
```

Change the TitleID in the makefile and make sure the appropriate named json exists as well.

## Credits

- [exlaunch](https://github.com/shadowninja108/exlaunch/)
