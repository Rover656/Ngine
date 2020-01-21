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

#include "Game.hpp"

#include "Audio/AudioDevice.hpp"
#include "Input/Gamepad.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
#include "Console.hpp"

#if defined(PLATFORM_UWP)
#include "UWP/GameApp.hpp"
#endif

namespace NerdThings::Ngine {
    void Game::_init() {
        // Create window
        m_gameWindow = new Window(m_gameWindowCreationConfig);
        Console::Notice("Game", "Created game window.");

        // Create graphics device.
        m_graphicsDevice = new Graphics::GraphicsDevice(m_gameWindow);
        Console::Notice("Game", "Created graphics device.");

        // Create renderer.
        m_renderer = new Graphics::Renderer(m_graphicsDevice);
        Console::Notice("Game", "Created renderer.");

        // Init Gamepad
        Input::Gamepad::Init();
        Console::Notice("Game", "Gamepad API has been initialized.");

        // Init audio
        Console::Notice("Game", "Attempting to initialize audio device.");
        Audio::AudioDevice::Initialize();

        // Check if the device was created
        if (Audio::AudioDevice::IsReady())
            Console::Notice("Game", "Audio device initialized successfully.");
        else Console::Warn("Game", "Failed to create audio device. Audio APIs will be unavailable.");

        // Create render target
        // TODO: Rework this to be more robust.
        if (Config.MaintainResolution) {
            m_renderTarget = new Graphics::RenderTexture(m_graphicsDevice, Config.TargetWidth, Config.TargetHeight);
            m_renderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
        }

        // Create resource manager
        Console::Notice("Game", "Creating the game resource manager.");
        m_resourceManager = new Filesystem::ResourceManager(m_graphicsDevice);

        // DEV: If anything new is added to the Game class, its initialization should occur here (Before OnInit())

        // Invoke OnRun
        OnInit();

        // Init Timing
        m_lag = std::chrono::nanoseconds(0);
        m_started = std::chrono::high_resolution_clock::now();
        m_timestep = std::chrono::milliseconds(int(1000.0f / float(Config.UpdatesPerSecond)));
    }

    void Game::_runDraw() {
        // Poll events
        m_gameWindow->PollEvents();

        // Don't render if suspended
        if (m_suspended) {
            // Do nothing this frame.
            std::this_thread::sleep_for(m_timestep);
            return;
        }

        // Make our window current so we can render to it.
        m_gameWindow->MakeCurrent();

        // Window/Game Size variables
        const auto w = static_cast<float>(m_gameWindow->GetWidth());
        const auto h = static_cast<float>(m_gameWindow->GetHeight());
        const auto iw = static_cast<float>(Config.TargetWidth);
        const auto ih = static_cast<float>(Config.TargetHeight);
        const auto scale = std::min(w / iw, h / ih);
        const auto offsetX = (w - iw * scale) * 0.5f;
        const auto offsetY = (h - ih * scale) * 0.5f;

        // TODO: Clean this mess
        if (Config.MaintainResolution && m_renderTarget == nullptr) {
            m_renderTarget = new Graphics::RenderTexture(m_graphicsDevice, Config.TargetWidth, Config.TargetHeight);
            m_renderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
        } else if (Config.MaintainResolution && (!m_renderTarget->IsValid() ||
                                                 (m_renderTarget->Width != Config.TargetWidth ||
                                                  m_renderTarget->Height != Config.TargetHeight))) {
            m_renderTarget = new Graphics::RenderTexture(m_graphicsDevice, Config.TargetWidth, Config.TargetHeight);
            m_renderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
        }

        if (Config.MaintainResolution && m_renderTarget != nullptr)
            m_renderTarget->GetTexture()->SetTextureFilter(RenderTargetFilterMode);

        // Get the time that we begin (for timing)
        auto frameBegin = std::chrono::high_resolution_clock::now();

        // Setup mouse translation
        if (Config.MaintainResolution && m_renderTarget->IsValid()) {
            const auto mouse = m_gameWindow->GetMouse();
            mouse->SetScale(iw / (w - offsetX * 2.0f), ih / (h - offsetY * 2.0f));
            mouse->SetOffset(-offsetX, -offsetY);
        }

        // Only render if visible
        if (m_gameWindow->IsVisible()) {
            // If using, start render target
            if (Config.MaintainResolution && m_renderTarget->IsValid()) {
                // Clear the main framebuffer (for black bars)
                m_renderer->SetClearColor(Graphics::Color::Black);
                m_renderer->Clear();

                // Enable our render target
                m_graphicsDevice->PushTarget(m_renderTarget);
            }

            // Clear with clear color
            m_renderer->SetClearColor(ClearColor);
            m_renderer->Clear();

            // Render scene
            if (m_currentScene != nullptr) {
                m_currentScene->Draw(m_renderer);
            }

            // OnDraw event
            OnDraw(m_renderer);

            // If using a target, draw target
            if (Config.MaintainResolution && m_renderTarget->IsValid()) {
                m_graphicsDevice->PopTarget();
                m_renderTarget->GetTexture()->Draw(
                        m_renderer,
                        {
                                (w - iw * scale) * 0.5f,
                                (h - ih * scale) * 0.5f,
                                iw * scale,
                                ih * scale
                        },
                        {
                                0,
                                0,
                                static_cast<float>(m_renderTarget->Width),
                                static_cast<float>(m_renderTarget->Height) * -1
                        },
                        Graphics::Color::White);
            }

            // Render final buffers
            m_renderer->Render();

            // Swap buffers
            m_gameWindow->SwapBuffers();
        }

        // FPS limiter
        auto frameTime = std::chrono::high_resolution_clock::now() - frameBegin;
        auto frameTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime);
        auto timestep = std::chrono::milliseconds((int)(1000.0f / (float) Config.FPSCap));

        if (frameTimeMS < timestep) {
            std::this_thread::sleep_for(timestep - frameTimeMS);
        }
    }

    void Game::_runUpdate() {
        // Check if we are/should be suspended
        if (!m_gameWindow->IsVisible() && !Config.RunWhileHidden) {
            // If told to quit, quit
            if (!m_running) return;

            // Suspend if we haven't
            if (!m_suspended) {
                OnSuspend();
                m_suspended = true;
            }

            // Do nothing this frame.
            std::this_thread::sleep_for(m_timestep);
        } else {
            // Resume if we had suspended
            if (m_suspended) {
                OnResume();
                m_suspended = false;
            }

            // Update timestep if FPS has changed
            if (m_timestep.count() != int(1000.0f / (float) Config.UpdatesPerSecond)) {
                m_timestep = std::chrono::milliseconds(int(1000.0f / (float) Config.UpdatesPerSecond));
                Console::Notice("Game", "Timestep updated to match updates per second.");
            }

            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - m_started;
            m_started = std::chrono::high_resolution_clock::now();

            // Increment lag
            m_lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Get the time that we begin (for timing)
            auto frameBegin = std::chrono::high_resolution_clock::now();

            // Get input managers
            auto mouse = GetMouse();
            auto keyboard = GetKeyboard();

            // Setup mouse translation
            if (Config.MaintainResolution && m_renderTarget->IsValid()) {
                // Get fields required for ofsetting.
                const auto w = (float) m_gameWindow->GetWidth();
                const auto h = (float) m_gameWindow->GetHeight();
                const auto iw = (float) Config.TargetWidth;
                const auto ih = (float) Config.TargetHeight;
                const auto scale = std::min(w / iw, h / ih);
                const auto offsetX = (w - iw * scale) * 0.5f;
                const auto offsetY = (h - ih * scale) * 0.5f;

                mouse->SetScale(iw / (w - offsetX * 2.0f), ih / (h - offsetY * 2.0f));
                mouse->SetOffset(-offsetX, -offsetY);
            }

            // TODO: Detect if we are behind on updates.

            // Poll inputs if window is visible and if we're going to update this frame
            if (m_lag >= m_timestep) {
                Input::Gamepad::PollInputs();
                mouse->PollInputs();
                keyboard->PollInputs();
            }

            // Run required updates
            while (m_lag >= m_timestep) {
                // Run a single update
                m_lag -= m_timestep;

                // Run an update
                OnUpdate();

                // Update the current scene.
                if (m_currentScene != nullptr) {
                    m_currentScene->Update();
                }

                // AudioDevice update
                Audio::AudioDevice::Update();

                // If we need to quit, don't run any more frames
                if (!m_running) return;
            }

            // If we need to quit, don't render
            if (!m_running) return;

            // Wait for remaining frame time if we were too quick
            auto frameTime = std::chrono::high_resolution_clock::now() - frameBegin;
            auto frameTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime);

            if (frameTimeMS < m_timestep) {
                std::this_thread::sleep_for(m_timestep - frameTimeMS);
            }
        }
    }

    void Game::_updateThread() {
        while (m_running) {
            _runUpdate();
        }
    }

    void Game::_cleanup() {
        // Delete render target now so that it doesnt try after GL is gone.
        m_renderTarget = nullptr;

        // Call the suspend event.
        OnSuspend();

        // Call the cleanup event
        OnCleanup();

        // Delete resource manager
        delete m_resourceManager;

        // Close audio
        Audio::AudioDevice::Close();
        Console::Notice("Game", "Closed audio device.");

        // Delete render target
        delete m_renderTarget;
        m_renderTarget = nullptr;

        // Delete renderer
        delete m_renderer;
        m_renderer = nullptr;
        Console::Notice("Game", "Deleted renderer.");

        // Delete graphics device
        delete m_graphicsDevice;
        m_graphicsDevice = nullptr;
        Console::Notice("Game", "Deleted graphics device.");

        // Close window
        delete m_gameWindow;
        m_gameWindow = nullptr;
        Console::Notice("Game", "Deleted game window.");
    }

    Game::Game(WindowConfig windowConfig_, const GameConfig &config_)
            : m_gameWindowCreationConfig(std::move(windowConfig_)), Config(config_) {}

    Window *Game::GetGameWindow() const {
        return m_gameWindow;
    }

    Input::Mouse *Game::GetMouse() const {
        return m_gameWindow ? m_gameWindow->GetMouse() : nullptr;
    }

    Input::Keyboard *Game::GetKeyboard() const {
        return m_gameWindow ? m_gameWindow->GetKeyboard() : nullptr;
    }

    Graphics::GraphicsDevice *Game::GetGraphicsDevice() {
        return m_graphicsDevice;
    }

    Vector2 Game::GetDefaultWindowDimensions() const {
        return {(float) m_gameWindow->GetWidth(), (float) m_gameWindow->GetHeight()};
    }

    Vector2 Game::GetDimensions() const {
        return {(float) Config.TargetWidth, (float) Config.TargetHeight};
    }

    int Game::GetTargetFPS() const {
        return Config.UpdatesPerSecond;
    }

    void Game::Run() {
#if defined(PLATFORM_UWP)
        throw std::runtime_error("For the UWP platform, the Run() method must not be used! Use UWP::GameApp instead.");
#else
        // Init game.
        _init();

        // Mark as running
        m_running = true;

        // Start update thread
        std::thread updateThread(&Game::_updateThread, this);

        // This checks the game should still run
        while (!m_gameWindow->ShouldClose() && m_running) {
            // Draw frame
            _runDraw();
        }

        // No longer running
        m_running = false;

        // Join update thread back into main
        updateThread.join();

        // Clean up
        _cleanup();
#endif
    }

    bool Game::IsRunning() {
        return m_running;
    }

    void Game::Quit() {
        m_running = false;
    }

    void Game::SetFPS(int FPS_) {
        Config.UpdatesPerSecond = FPS_;
    }

    void Game::SetIntendedSize(Vector2 size_) {
        Config.TargetWidth = (int) size_.X;
        Config.TargetHeight = (int) size_.Y;
    }

    void Game::SetIsRunning(bool running_) {
        m_running = running_;
    }

    void Game::SetScene(Scene *scene_) {
        if (m_currentScene != nullptr)
            m_currentScene->OnUnload({this});

        m_currentScene = scene_;

        if (m_currentScene != nullptr)
            m_currentScene->OnInit({this});

        Console::Notice("Game", "A new scene has been loaded.");
    }

    Filesystem::ResourceManager *Game::GetResourceManager() const {
        return m_resourceManager;
    }
}