<h1 align="center"><a href="https://github.com/NerdThings/Ngine"><img src="https://directus.nerdthings.dev/uploads/nerdthings/originals/62f51205-2704-4c0e-be5e-071fb4f4ac67.svg" height="200px"/></a></h1>

<p align="center">
Ngine is a C++ 17 library for producing 2D games.
</p>

<p align="center">
    <a href="https://travis-ci.org/NerdThings/Ngine"><img alt="Travis (.org)" src="https://img.shields.io/travis/NerdThings/Ngine?label=Linux%20%28master%29&logo=travis&style=for-the-badge"></a>
    <a href="https://ci.appveyor.com/project/Rover656/ngine/branch/master"><img alt="AppVeyor" src="https://img.shields.io/appveyor/build/Rover656/ngine/master?label=Windows%20%28master%29&logo=appveyor&style=for-the-badge"></a>
    <a href="https://discord.nerdthings.dev"><img alt="Discord" src="https://img.shields.io/discord/452810843852374016?label=Discord&logo=discord&style=for-the-badge"></a>
    <a href="https://choosealicense.com/licenses/apache-2.0/"><img alt="GitHub" src="https://img.shields.io/github/license/NerdThings/Ngine?logo=apache&style=for-the-badge"></a>
    <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/last-commit/NerdThings/Ngine/develop?label=Last%20Commit%20%28develop%29&logo=github&style=for-the-badge">
</p>

## Features

- Ngine supports Windows, Linux and UWP.
- Ngine has a comprehensive renderer which is consistent across graphics APIs.
- Ngine provides a simple to use, feature filled entity-component system, allowing complete control over your scenes.
- Ngine exposes the rendering process to the developer with virtual methods, allowing complete control over how your scenes are rendered.

## Installation

You must build Ngine from source, Ngine uses [CMake](https://cmake.org/download/) (3.12 minimum) to generate project files. Use this to generate build files for the default target. See the [Wiki](https://github.com/NerdThings/Ngine/wiki/) for more detailed instructions for your platform/target.

```
mkdir build & cd build
cmake ..
```

## Usage

This shows you how to launch your game class (which you have to write I'm afraid)...

If you want more comprehensive usage examples, we're working on some right now, see the examples directory for some!

```c++
#include <Ngine.hpp>

NGINE_GAME_ENTRY {
    // Game config
    ngine::GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    
    // Window config
    ngine::WindowConfig windowConfig;
    windowConfig.Resizable = true;
    windowConfig.InitialWidth = 1280;
    windowConfig.InitialHeight = 768;
    
    // Create game
    auto game = YourGameClass(gameConfig, windowConfig);
    
    NGINE_RUN_GAME(game);
    
    return 0;
}
```

## Built with

- [ANGLE](https://chromium.googlesource.com/angle/angle) - DirectX support through OpenGL translation.
- [Box2D](https://github.com/erincatto/box2d) - The 2D physics engine.
- [glad](https://glad.dav1d.de/) - OpenGL extension loading.
- [miniaudio](https://github.com/dr-soft/miniaudio) - Cross platform audio library.
- [stb](https://github.com/nothings/stb) - stb_image for image loading and stb_truetype for font handling.

## Contributing
Pull requests are welcome. For major changes, check if it is an approved feature on the projects tab. If it isn't, please open an issue or chat with us on Discord.

## License
[Apache 2.0](https://choosealicense.com/licenses/apache-2.0/)
