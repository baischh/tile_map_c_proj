# C platformer for TI 84 Plus CE graphing calculator

This is a platformer style game built using the CE C/C++ Toolchain for Linux. 

## Building the game
To build the game you must have the CE Toolchain installed, then run the following commands:

`cd src/gfx`  
`convimg`

These commands convert the image inputs according to the convimg.yaml config

From the project root `make` can be used to build the .8xp executable for the calculator.

Use a tool like Connect CE to send the file to the calulator or an emulator like CEmu to play the game.
The required lobraries can be found at [the CE Toolchain website](https://ce-programming.github.io/toolchain/index.html)

### Multiple live enemies
Include multiple moving enemies spawned from minimal enemy template, MAX_ACTIVE_ENEMIES = 10

![Game img](/game_images/game_play.png)

### Sprite animations
Currently the fire character sprite is the only enimated sprite.

![Animated Sprites](/game_images/animated_enemies.png)
