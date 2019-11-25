/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Game.h"

#include "Audio/AudioDevice.h"
#include "Graphics/GraphicsManager.h"
#include "Input/Gamepad.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Filesystem/ResourceManager.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)

#include "Graphics/OpenGL/OpenGL.h"

#endif

#if defined(PLATFORM_UWP)
#include "UWP/GameApp.h"
#endif

namespace NerdThings::Ngine {
    void Game::_doDraw() {
#ifdef USE_EXPERIMENTAL_RENDERER
#else
        // Prep for drawing
        Graphics::Renderer::BeginDrawing();

        // If using, start using target
        if (Config.MaintainResolution && m_renderTarget->IsValid()) {
            // Clear the main framebuffer (for black bars)
            Graphics::Renderer::Clear(Graphics::Color::Black);

            // Enable our main framebuffer
            Graphics::GraphicsManager::PushTarget(m_renderTarget.get());
        }

        // Clear with the correct background colour
        Graphics::Renderer::Clear(ClearColor);

        // Render scene
        if (m_currentScene != nullptr) {
            m_currentScene->Draw();
        }
#endif
        // OnDraw event
        OnDraw();
    }

    void Game::_doUpdate() {
        // Run update events
        OnUpdate();

        if (m_currentScene != nullptr) {
            m_currentScene->Update();
        }

        // AudioDevice update
        Audio::AudioDevice::Update();
    }

    Game::Game(WindowConfig windowConfig_, const GameConfig &config_)
            : m_gameWindowCreationConfig(std::move(windowConfig_)), Config(config_) {}

    Window *Game::GetGameWindow() const {
        return m_gameWindow;
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
        return Config.FPS;
    }

    bool Game::IsRunning() {
        return m_running;
    }

    void Game::Quit() {
        m_running = false;
    }

    void Game::Run() {
        // Create window
        m_gameWindow = new Window(m_gameWindowCreationConfig);
        ConsoleMessage("Created game window.", "NOTICE", "Game");

        // Create graphics device.
        m_graphicsDevice = new Graphics::GraphicsDevice(m_gameWindow);
        ConsoleMessage("Created graphics device.", "NOTICE", "Game");

        // Init Input
        Input::Gamepad::Init();
        Input::Mouse::Init();
        Input::Keyboard::Init();
        ConsoleMessage("Input APIs have been initialized.", "NOTICE", "Game");

        // Init audio
        ConsoleMessage("Attempting to initialize audio device.", "NOTICE", "Game");
        Audio::AudioDevice::Initialize();

        // Check if the device was created
        if (Audio::AudioDevice::IsReady())
            ConsoleMessage("Audio device initialized successfully..", "NOTICE", "Game");
        else ConsoleMessage("Failed to create audio device. Audio APIs will be unavailable.", "WARN", "Game");

        // Create render target
        if (Config.MaintainResolution) {
            m_renderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
            m_renderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
        }

        // Invoke OnRun
        OnInit();

        // Start game
        m_running = true;

        // Timing
        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();
        auto lastFPS = Config.FPS;
        auto timeStep = std::chrono::milliseconds(int(1000.0f / float(lastFPS)));

        // This checks the game is still running, the window is still running and the exit key is not pushed
        while (!m_gameWindow->ShouldClose() && !Input::Keyboard::ShouldClose()) {
            // Update timestep if FPS has changed
            if (Config.FPS != lastFPS) {
                lastFPS = Config.FPS;
                timeStep = std::chrono::milliseconds(int(1000.0f / float(lastFPS)));
                ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "Game");
            }

            if (!m_gameWindow->IsVisible() && !Config.RunWhileHidden) {
                // Poll window events
                m_gameWindow->PollEvents();

                // If told to quit, quit
                if (!m_running) break;

                // Do nothing this frame.
                std::this_thread::sleep_for(timeStep);
                continue;
            }

            // Make our window current.
            m_gameWindow->MakeCurrent();

            // Window/Game Size variables
            const auto w = static_cast<float>(m_gameWindow->GetWidth());
            const auto h = static_cast<float>(m_gameWindow->GetHeight());
            const auto iw = static_cast<float>(Config.TargetWidth);
            const auto ih = static_cast<float>(Config.TargetHeight);
            const auto scale = std::min(w / iw, h / ih);
            const auto offsetX = (w - iw * scale) * 0.5f;
            const auto offsetY = (h - ih * scale) * 0.5f;

            if (Config.MaintainResolution && m_renderTarget == nullptr) {
                m_renderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
                m_renderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
            } else if (Config.MaintainResolution && (!m_renderTarget->IsValid() ||
                                                     (m_renderTarget->Width != Config.TargetWidth ||
                                                      m_renderTarget->Height != Config.TargetHeight))) {
                m_renderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
                m_renderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
            }

            if (Config.MaintainResolution && m_renderTarget == nullptr)
                m_renderTarget->GetTexture()->SetTextureFilter(RenderTargetFilterMode);

            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - started;
            started = std::chrono::high_resolution_clock::now();

            // Increment lag
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Get the time that we begin (for timing)
            auto frameBegin = std::chrono::high_resolution_clock::now();

            // Setup mouse translation
            if (Config.MaintainResolution && m_renderTarget->IsValid()) {
                Input::Mouse::SetScale(iw / (w - offsetX * 2.0f), ih / (h - offsetY * 2.0f));
                Input::Mouse::SetOffset(-offsetX, -offsetY);
            }

            // If we are lagging more than 5 seconds, don't count any further
            if (lag.count() >= 5e+9) lag = std::chrono::nanoseconds(0);

            // Poll inputs if window is visible and if we're going to update this frame
            if (m_gameWindow->IsFocussed() && m_gameWindow->IsVisible() && lag >= timeStep) {
                Input::Gamepad::PollInputs();
                Input::Mouse::PollInputs();
                Input::Keyboard::PollInputs();
            }

            // Run required updates
            while (lag >= timeStep) {
                // Run a single update
                lag -= timeStep;
                _doUpdate();

                // If we need to quit, don't run any more frames
                if (!m_running) break;
            }

            // If we need to quit, don't render
            if (!m_running) break;

            // Only render if visible
            if (m_gameWindow->IsVisible()) {
                // Draw
                _doDraw();

                // TODO: Deal with all the const variables so this can live in _doDraw()

                // If using a target, draw target
                if (Config.MaintainResolution && m_renderTarget->IsValid()) {
#ifdef USE_EXPERIMENTAL_RENDERER
#else
                    Graphics::GraphicsManager::PopTarget();
                    Graphics::Renderer::DrawTexture(m_renderTarget->GetTexture(),
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
#endif
                }

                // Finish drawing
#ifdef USE_EXPERIMENTAL_RENDERER
#else
                Graphics::Renderer::EndDrawing();
#endif

                // Swap buffers
                m_gameWindow->SwapBuffers();
            }

            // Reset mouse
            if (Config.MaintainResolution && m_renderTarget->IsValid()) {
                Input::Mouse::SetScale(1, 1);
                Input::Mouse::SetOffset(0, 0);
            }

            // Poll events
            m_gameWindow->PollEvents();

            // Wait for remaining frame time if we were too quick
            auto frameTime = std::chrono::high_resolution_clock::now() - frameBegin;
            auto frameTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime);

            if (frameTimeMS < timeStep) {
                std::this_thread::sleep_for(timeStep - frameTimeMS);
            }

            if (!m_running) break;
        }

        // Delete render target now so that it doesnt try after GL is gone.
        m_renderTarget = nullptr;

        // Call the cleanup event
        OnCleanup();

        // Close audio
        ConsoleMessage("Closing audio device.", "NOTICE", "Window");
        Audio::AudioDevice::Close();

        // Delete graphics device
        delete m_graphicsDevice;
        m_graphicsDevice = nullptr;

        // Close window
        delete m_gameWindow;
        m_gameWindow = nullptr;
    }

    void Game::SetFPS(int FPS_) {
        Config.FPS = FPS_;
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
            m_currentScene->OnUnLoad({this});

        m_currentScene = scene_;

        if (m_currentScene != nullptr)
            m_currentScene->OnLoad({this});

        ConsoleMessage("A new scene has been loaded.", "NOTICE", "Game");
    }
}