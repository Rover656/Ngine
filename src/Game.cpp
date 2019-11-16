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
#include "Filesystem/Resources.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "Graphics/OpenGL/OpenGL.h"
#endif

#if defined(PLATFORM_UWP)
#include "UWP/GameApp.h"
#endif

namespace NerdThings::Ngine {
    // Private Methods

    void Game::__DoDraw() {
        // Prep for drawing
        Graphics::Renderer::BeginDrawing();

        // If using, start using target
        if (Config.MaintainResolution && _RenderTarget->IsValid()) {
            // Clear the main framebuffer (for black bars)
            Graphics::Renderer::Clear(Graphics::Color::Black);

            // Enable our main framebuffer
            Graphics::GraphicsManager::PushTarget(_RenderTarget.get());
        }

        // Clear with the correct background colour
        Graphics::Renderer::Clear(ClearColor);

        // Render scene
        if (_CurrentScene != nullptr) {
            _CurrentScene->Draw();
        }

        // OnDraw event
        OnDraw();
    }

    void Game::__DoUpdate() {
        // Run update events
        OnUpdate();

        if (_CurrentScene != nullptr) {
            _CurrentScene->Update();
        }

        // AudioDevice update
        Audio::AudioDevice::Update();
    }

    // Public Constructor(s)

    Game::Game(const GameConfig &config_) {
        // Save config
        Config = config_;
    }

    // Destructor

    Game::~Game() = default;

    // Public Methods

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
        // Initialize window
        Window::Init();
        ConsoleMessage("Window has been initialized.", "NOTICE", "Game");

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
            _RenderTarget = std::make_shared<Graphics::RenderTarget>(Config.TargetWidth, Config.TargetHeight);
            _RenderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
        }

        // Invoke OnRun
        OnInit();

        // Start game
        _Running = true; // TODO: We have too many of these kind of variables

        // Timing
        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();
        auto lastFPS = Config.FPS;
        auto timeStep = std::chrono::milliseconds(int(1000.0f / float(lastFPS)));

        // This checks the game is still running, the window is still running and the exit key is not pushed
        while (!Window::ShouldClose() && !Input::Keyboard::ShouldClose()) {
            // Update timestep if FPS has changed
            if (Config.FPS != lastFPS) {
                lastFPS = Config.FPS;
                timeStep = std::chrono::milliseconds(int(1000.0f / float(lastFPS)));
                ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "Game");
            }

            if (!Window::Visible() && !Config.RunWhileHidden) {
                // Poll window events
                Window::PollEvents();

                // If told to quit, quit
                if (!_Running) break;

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
            const auto offsetX = (w - iw * scale) * 0.5f;
            const auto offsetY = (h - ih * scale) * 0.5f;

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

            // Poll inputs if window is visible and if we're going to update this frame
            if (Window::IsFocussed() && Window::Visible() && lag >= timeStep) {
                Input::Gamepad::PollInputs();
                Input::Mouse::PollInputs();
                Input::Keyboard::PollInputs();
            }

            // Run required updates
            while (lag >= timeStep) {
                // Run a single update
                lag -= timeStep;
                __DoUpdate();

                // If we need to quit, don't run any more frames
                if (!_Running) break;
            }

            // If we need to quit, don't render
            if (!_Running) break;

            // Only render if visible
            if (Window::Visible()) {
                // Draw
                __DoDraw();

                // TODO: Deal with all the const variables so this can live in __DoDraw()

                // If using a target, draw target
                if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                    Graphics::GraphicsManager::PopTarget();
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

            if (frameTimeMS < timeStep) {
                std::this_thread::sleep_for(timeStep - frameTimeMS);
            }

            if (!_Running) break;
        }

        // Delete render target now so that it doesnt try after GL is gone.
        _RenderTarget = nullptr;

        // Delete loaded resources
        Filesystem::Resources::DeleteAll();

        // Close audio
        ConsoleMessage("Closing audio device.", "NOTICE", "Window");
        Audio::AudioDevice::Close();

        // Close window
        Window::Close();
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
}