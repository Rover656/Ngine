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
// Ngine includes
// ---------------------------------------------------------------------------------

#include "audio/AudioBuffer.hpp"
#include "audio/AudioDevice.hpp"
#include "audio/AudioStream.hpp"
#include "audio/Music.hpp"
#include "audio/Sound.hpp"
#include "audio/Wave.hpp"
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
#include "Graphics/VertexDataConverter.hpp"
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
#define NGINE_RUN_GAME(game) game.run();
#endif

#endif // NGINE_HPP
