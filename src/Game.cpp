/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
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
#include "Filesystem/Resources.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "Graphics/OpenGL/OpenGL.h"
#endif

#if defined(PLATFORM_UWP)
#include "Platform/UWP/GameApp.h"
#endif

namespace NerdThings::Ngine {
    // Public Constructor(s)

    Game::Game(const GameConfig &config_) {
        // Save config
        Config = config_;

#if !defined(PLATFORM_UWP) // UWP init elsewhere
        // Initialize window
        Window::Init();
        ConsoleMessage("Window has been initialized.", "NOTICE", "Game");
#endif
    }

    // Destructor

    Game::~Game() {}

    // Public Methods

    void Game::Clear() const {
        Graphics::Renderer::Clear(ClearColor);
    }

    void Game::Draw() {
        if (_CurrentScene != nullptr) {
            OnDraw({});
            _CurrentScene->Draw();
        }
    }

    Vector2 Game::GetDefaultWindowDimensions() const {
        return { (float)Window::GetWidth(), (float)Window::GetHeight() };
    }

    Vector2 Game::GetDimensions() const {
        return { (float)Config.TargetWidth, (float)Config.TargetHeight };
    }

    int Game::GetTargetFPS() const {
        return Config.FPS;
    }

    bool Game::IsRunning() {
        return _Running;
    }

    void Game::Quit() {
        _Running = false;
    }

    void Game::Run() {
        // Create render target
        if (Config.MaintainResolution) {
            _RenderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
            _RenderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
        }

        // Init audio
        ConsoleMessage("Attempting to initialize audio device.", "NOTICE", "Game");
        Audio::AudioDevice::Initialize();

        // Check if the device was created
        if (Audio::AudioDevice::IsReady())
            ConsoleMessage("Audio device initialized successfully..", "NOTICE", "Game");
        else ConsoleMessage("Failed to create audio device, audio will be unavailable.", "WARN", "Game");

        // Invoke OnRun
        OnRun({});

        // Start game
        _Running = true;

        // Timing
        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();
        auto lastFPS = Config.FPS;
        auto timeStep = std::chrono::milliseconds(int(1000.0f / float(lastFPS)));

        // This checks the game is still running, the window is still running and the exit key is not pushed
        while (!Window::ShouldClose() && !Input::Keyboard::ShouldClose() && _Running) {
            // Update timestep if FPS has changed
            if (Config.FPS != lastFPS) {
                lastFPS = Config.FPS;
                timeStep = std::chrono::milliseconds(int(1000.0f / float(lastFPS)));
                ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "Game");
            }

            if (!Window::Visible() && !Config.RunWhileHidden) {
                // Do nothing this frame.
                std::this_thread::sleep_for(timeStep);
                continue;
            }

            // Window/Game Size variables
            const auto w = static_cast<float>(Window::GetWidth());
            const auto h = static_cast<float>(Window::GetHeight());
            const auto iw = static_cast<float>(Config.TargetWidth);
            const auto ih = static_cast<float>(Config.TargetHeight);
            const auto scale = std::min(w / iw, h / ih);
            const auto offsetX = (w - iw * scale) * 0.5;
            const auto offsetY = (h - ih * scale) * 0.5;

            if (Config.MaintainResolution && _RenderTarget == nullptr) {
                _RenderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
                _RenderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
            } else if (Config.MaintainResolution && (!_RenderTarget->IsValid() || (_RenderTarget->Width != Config.TargetWidth || _RenderTarget->Height != Config.TargetHeight))) {
                _RenderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
                _RenderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
            }

            _RenderTarget->GetTexture()->SetTextureFilter(RenderTargetFilterMode);

            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - started;
            started = std::chrono::high_resolution_clock::now();

            // Increment lag
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Get the time that we begin (for timing)
            auto frameBegin = std::chrono::high_resolution_clock::now();

            // Setup mouse translation
            if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                Input::Mouse::SetScale(iw / (w - offsetX * 2.0f), ih / (h - offsetY * 2.0f));
                Input::Mouse::SetOffset(-offsetX, -offsetY);
            }

            // If we are lagging more than 5 seconds, don't count any further
            if (lag.count() >= 5e+9) lag = std::chrono::nanoseconds(0);

            // Poll inputs if window is visible only.
            if (Window::Visible()) { // TODO: This needs to detect window that is not focussed
                Input::Gamepad::PollInputs();
                Input::Mouse::PollInputs();
                Input::Keyboard::PollInputs();
            }

            // Run required updates
            while (lag >= timeStep) {
                // Run a single update
                lag -= timeStep;
                Update();
            }

            // Only render if visible
            if (Window::Visible()) {
                // Prep for drawing
                Graphics::Renderer::BeginDrawing();

                // Clear
                Graphics::Renderer::Clear(Graphics::Color::Black);

                // If using, start using target
                if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                    Graphics::GraphicsManager::PushTarget(_RenderTarget.get());
                }

                // Clear the background
                Clear();

                // Draw
                Draw();

                // If using a target, draw target
                if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                    auto popped = false;
                    Graphics::GraphicsManager::PopTarget(popped);

                    Graphics::Renderer::DrawTexture(_RenderTarget->GetTexture(),
                                                    {
                                                            (w - iw * scale) * 0.5f,
                                                            (h - ih * scale) * 0.5f,
                                                            iw * scale,
                                                            ih * scale
                                                    },
                                                    {
                                                            0,
                                                            0,
                                                            static_cast<float>(_RenderTarget->Width),
                                                            static_cast<float>(_RenderTarget->Height) * -1
                                                    },
                                                    Graphics::Color::White);
                }

                // Finish drawing
                Graphics::Renderer::EndDrawing();

                // Swap buffers
                Window::SwapBuffers();
            }

            // Reset mouse
            if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                Input::Mouse::SetScale(1, 1);
                Input::Mouse::SetOffset(0, 0);
            }

            // Poll events
            Window::PollEvents();

            // Wait for remaining frame time if we were too quick
            auto frameTime = std::chrono::high_resolution_clock::now() - frameBegin;
            auto frameTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime);

            // TODO: Work out why we drop about 2 frames...
            if (frameTimeMS < timeStep) {
                std::this_thread::sleep_for(timeStep - frameTimeMS);
            }
        }

        // Delete render target now so that it doesnt try after GL is gone.
        _RenderTarget = nullptr;

#if !defined(PLATFORM_UWP) // UWP handles this elsewhere because this is ignored
        // Delete loaded resources
        Filesystem::Resources::DeleteAll();

        // Close audio
        ConsoleMessage("Closing audio device.", "NOTICE", "Game");
        Audio::AudioDevice::Close();

        // Close window
        ConsoleMessage("Closing window.", "NOTICE", "Game");
        Window::Close();

        ConsoleMessage("Game successfully stopped.", "NOTICE", "Game");
#endif
    }

    void Game::SetFPS(int FPS_) {
        Config.FPS = FPS_;
    }

    void Game::SetIntendedSize(Vector2 size_) {
        Config.TargetWidth = (int)size_.X;
        Config.TargetHeight = (int)size_.Y;
    }

    void Game::SetIsRunning(bool running_) {
        _Running = running_;
    }

    void Game::SetScene(Scene *scene_) {
        if (_CurrentScene != nullptr)
            _CurrentScene->OnUnLoad({this});

        _CurrentScene = scene_;

        if (_CurrentScene != nullptr)
            _CurrentScene->OnLoad({this});

        ConsoleMessage("A new scene has been loaded.", "NOTICE", "Game");
    }

    void Game::Update() {
        // Run update events
        OnUpdate({});

        if (_CurrentScene != nullptr) {
            _CurrentScene->Update();
        }

        // AudioDevice update
        Audio::AudioDevice::Update();
    }
}