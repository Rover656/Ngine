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
#include "components/SpriteComponent.hpp"
#include "filesystem/Filesystem.hpp"
#include "filesystem/ResourceManager.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Color.hpp"
#include "graphics/Font.hpp"
#include "graphics/GraphicsDevice.hpp"
#include "graphics/Image.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/RenderTarget.hpp"
#include "graphics/Shader.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/ShaderProgramState.hpp"
#include "graphics/ShapeRenderer.hpp"
#include "graphics/Sprite.hpp"
#include "graphics/Texture2D.hpp"
#include "graphics/VertexDataConverter.hpp"
#include "graphics/VertexLayout.hpp"
#include "input/Gamepad.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "physics/PhysicsBody.hpp"
#include "physics/PhysicsContext.hpp"
#include "physics/PhysicsFixture.hpp"
#include "physics/PhysicsShape.hpp"
#include "physics/PhysicsWorld.hpp"
// TODO: UI once the API is finished
#include "Component.hpp"
#include "Console.hpp"
#include "Entity.hpp"
#include "Events.hpp"
#include "Game.hpp"
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
