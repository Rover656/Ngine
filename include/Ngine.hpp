/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef NGINE_HPP
#define NGINE_HPP

#include "Config.hpp"

// ---------------------------------------------------------------------------------
// Doxygen Pages
// ---------------------------------------------------------------------------------

/**
 * @mainpage Ngine API Documentation
 *
 * @section intro_sec Introduction
 * Ngine is a 2D game engine written in C++.
 *
 * @section installation Installation
 * NerdThings does not currently provide binaries for Ngine, you must build it from source instead.
 * Build instructions are available on the @ref platform_support "Platform Support" page
 *
 * @page platform_support Platform Support
 * @tableofcontents
 *
 * Ngine supports a wide range of platforms. This page details build instructions for each platform.
 * All build instructions are compatible with the CMake config guides.
 *
 * @section platform_win Windows (Native)
 * Ngine primarily supports native Windows games and therefore Windows will have every feature (where applicable).
 *
 * @subsection platform_win_graphics Graphics APIs
 * Windows supports both OpenGL and DirectX (via ANGLE) graphics APIs.
 *
 * @subsection platform_win_build Building
 * To build, you must have at least Visual Studio 2017 installed with relevant workloads. Once installed, run the VS command line and run the following:
 * MinGW support is untested.
 * @code
 * mkdir build & cd build
 * cmake .. <Insert any CMake config here>
 * cmake --build . --config <Debug or Release>
 * @endcode
 *
 * @section platform_linux Linux
 * All Ngine features are available on Linux.
 *
 * @subsection platform_linux_graphics Graphics APIs
 * Linux only supports the OpenGL graphics API.
 *
 * @subsection platform_linux_build Building
 * To build on linux, it is recommended to use GCC 7 or later.
 * The following packages need installed (example here is for linux):
 * @code
 * sudo apt-get update
 * sudo apt-get install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
 * @endcode
 * Ngine can then be built with the following command:
 * @code
 * mkdir build & cd build
 * cmake -DCMAKE_BUILD_TYPE=<Release or Debug> <Other CMake config> ..
 * make
 * @endcode
 *
 * @section platform_mac Mac OS X
 * Ngine does not currently support Mac OS X and it is not planned. Feel free to work on this and publish a PR if you decide to try!
 *
 * @section platform_uwp Windows UWP (Desktop, Mobile, Xbox One)
 * Ngine supports UWP across Desktop and Xbox One. No Mobile UWP features will be added as Windows 10 Mobile is discontinued.
 *
 * @subsection platform_uwp_graphics Graphics APIs
 * Ngine on UWP uses DirectX (ANGLE).
 *
 * @subsection platform_uwp_missingfeatures Missing Features
 * - Ngine does not support touch input at the moment.
 *
 * @subsection platform_uwp_build Building
 * Building for Ngine requires Visual Studio 2017 (2019 may work, but is untested) with UWP workloads.
 *
 * Ngine includes a build script `BuildUWP.bat` which will generate a directory `build-uwp` with visual studio projects.
 * It is recommended to implement this script for your own projects for ease.
 *
 * @section platform_other Not listed?
 * If a platform is not listed, it means it has not been considered yet, feel free to ask about this on the GitHub page.
 * It is planned to add easier platform porting in the future (allowing community ports to be made easily).
 *
 * @page helpful_macros Helpful Macros
 * @tableofcontents
 *
 * Running an Ngine game works differently across different platforms, so Ngine provides the following macros to ensure that development is as easy as possible.
 *
 * @section macro_NGINE_GAME_ENTRY NGINE_GAME_ENTRY
 * NGINE_GAME_ENTRY defines the entrypoint for a program (i.e. int main() on linux).
 * This makes it easier to develop for multiple platforms as less code needs to be written specifically for a platform.
 *
 * @section macro_NGINE_RUN_GAME NGINE_RUN_GAME
 * NGINE_RUN_GAME runs a Ngine game for you. This is because on UWP, game running is more sophisticated. Using this macro makes your game compatible with UWP builds.
 */

//----------------------------------------------------------------------------------
// Namespace Documentation
//----------------------------------------------------------------------------------

/**
 * @namespace Ngine
 * The main Ngine namespace. Contains everything as part of the engine itself.
 *
 * @namespace Ngine::Audio
 * Ngine Audio module.
 *
 * @namespace Ngine::Components
 * This namespace consists of useful components.
 *
 * @namespace Ngine::Filesystem
 * Ngine Filesystem module.
 *
 * @namespace Ngine::Graphics
 * Ngine Graphics module.
 *
 * @namespace Ngine::Input
 * Ngine Input module.
 *
 * @namespace Ngine::Physics
 * Ngine Physics module.
 *
 * @namespace Ngine::Physics::Shapes
 * Physics shapes.
 *
 * @namespace Ngine::UWP
 * This namespace contains UWP specific code.
 */

// ---------------------------------------------------------------------------------
// Ngine includes
// ---------------------------------------------------------------------------------

#include "Audio/AudioBuffer.hpp"
#include "Audio/AudioDevice.hpp"
#include "Audio/AudioStream.hpp"
#include "Audio/Music.hpp"
#include "Audio/Sound.hpp"
#include "Audio/Wave.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/TilesetComponent.hpp"
#include "Filesystem/Filesystem.hpp"
#include "Filesystem/ResourceManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderSpace.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/ShapeRenderer.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Tileset.hpp"
#include "Graphics/TilesetRenderer.hpp"
#include "Graphics/VertexDataTool.hpp"
#include "Graphics/VertexLayout.hpp"
#include "Input/Gamepad.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
// TODO: Physics once we have finished the API in this branch
// TODO: UI once the API is finished
#include "Component.hpp"
#include "Entity.hpp"
#include "EntityContainer.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Console.hpp"
#include "Math.hpp"
#include "Rectangle.hpp"
#include "Resource.hpp"
#include "Scene.hpp"
#include "Window.hpp"

// ---------------------------------------------------------------------------------
// Platform specific features
// ---------------------------------------------------------------------------------

#if defined(PLATFORM_UWP)
#include "UWP/GameApp.hpp"
#endif

// ---------------------------------------------------------------------------------
// Handy macros
// ---------------------------------------------------------------------------------

/*
 * These macros are here to make cross platform coding easier.
 */

// Game entrypoint
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
#define NGINE_GAME_ENTRY int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
#define NGINE_GAME_ENTRY int main()
#endif
#elif defined(PLATFORM_UWP)
#define NGINE_GAME_ENTRY [Platform::MTAThread] int main(Platform::Array<Platform::String^>^)
#endif

// Game runner
#if defined(PLATFORM_UWP)
#include "UWP/GameApp.hpp"
#define NGINE_RUN_GAME(game) UWP::UWPGameBootstrapper::ExecuteGame(&game); CoreApplication::Run(ref new UWP::GameApplicationSource(ref new UWP::GameApp()));
#else
#define NGINE_RUN_GAME(game) game.Run();
#endif

#endif // NGINE_HPP