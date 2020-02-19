<h1 align="center">Ngine</h1>

<p align="center">
Ngine is a C++ 17 library for producing 2D games.

[![Build Status](https://travis-ci.org/NerdThings/Ngine.svg?branch=master)](https://travis-ci.org/NerdThings/Ngine)
[![Build status](https://ci.appveyor.com/api/projects/status/b02ab82sngs1m7ri/branch/master?svg=true)](https://ci.appveyor.com/project/Rover656/ngine/branch/master)
[![Chat on Discord](https://img.shields.io/discord/452810843852374016.svg?logo=discord)](https://discord.nerdthings.co.uk)
[![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://github.com/NerdThings/Ngine/blob/master/LICENSE)
![Platforms](https://img.shields.io/badge/platforms-Windows%20%7C%20Mac%20OS%20X%20%7C%20Linux-brightgreen.svg)
[![BCH compliance](https://bettercodehub.com/edge/badge/NerdThings/Ngine?branch=master)](https://bettercodehub.com/)
</p>

## Installation

You must build Ngine from source, Ngine uses [CMake](https://cmake.org/download/) (3.12 minimum) to generate project files. Use this to generate build files for the default target. See the [Wiki](https://github.com/NerdThings/Ngine/wiki/) for more detailed instructions for your platform/target.

```
mkdir build & cd build
cmake ..
```

## Usage

This shows you how to launch your game class (which you have to write I'm afraid)...

```c++
#include <Ngine.hpp>

NGINE_GAME_ENTRY {
    // Game config
    GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    
    // Window config
    WindowConfig windowConfig;
    windowConfig.Resizable = true;
    
    // Create game
    auto game = YourGameClass(gameConfig, windowConfig);
    
    NGINE_RUN_GAME(game);
    
    return 0;
}
```

## Contributing
Pull requests are welcome. For major changes, check if it is an approved feature on the projects tab. If it isn't, please open an issue or chat with us on Discord.

## License
[Apache 2.0](https://choosealicense.com/licenses/apache-2.0/)
