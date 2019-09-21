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
    // Public Fields

    Game *Game::CurrentGame = nullptr;

    // Public Constructor(s)

    Game::Game(const TGameConfig &config_) {
        // Check a game is not present
        if (CurrentGame != nullptr)
            throw std::runtime_error("Cannot create a second game before deleting the first!");
        CurrentGame = this;

        // Save config
        Config = config_;

#if !defined(PLATFORM_UWP) // UWP init elsewhere
        // Initialize window
        Window::Init();
        ConsoleMessage("Window has been initialized.", "NOTICE", "Game");
#endif
    }

    // Destructor

    Game::~Game() {
        CurrentGame = nullptr;
    }

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

    TVector2 Game::GetDefaultWindowDimensions() const {
        return { (float)Window::GetWidth(), (float)Window::GetHeight() };
    }

    TVector2 Game::GetDimensions() const {
        return { (float)Config.TargetWidth, (float)Config.TargetHeight };
    }

    int Game::GetDrawFPS() const {
        return Config.DrawFPS;
    }

    int Game::GetUpdateFPS() const {
        return Config.UpdateFPS;
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
            _RenderTarget = std::make_shared<Graphics::TRenderTarget>(Config.TargetWidth, Config.TargetHeight);
        }

        // Timing
        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();

        auto lastFPS = Config.UpdateFPS;
        auto timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

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

        while (!Window::ShouldClose() && _Running) {
            // Window/Game Size variables
            const auto w = static_cast<float>(Window::GetWidth());
            const auto h = static_cast<float>(Window::GetHeight());
            const auto iw = static_cast<float>(Config.TargetWidth);
            const auto ih = static_cast<float>(Config.TargetHeight);
            const auto scale = std::min(w / iw, h / ih);
            const auto offsetX = (w - iw * scale) * 0.5;
            const auto offsetY = (h - ih * scale) * 0.5;

            if (Config.MaintainResolution && _RenderTarget == nullptr) {
                _RenderTarget = std::make_shared<Graphics::TRenderTarget>(Config.TargetWidth, Config.TargetHeight);
            } else if (Config.MaintainResolution && (!_RenderTarget->IsValid() || (_RenderTarget->Width != Config.TargetWidth || _RenderTarget->Height != Config.TargetHeight))) {
                _RenderTarget = std::make_shared<Graphics::TRenderTarget>(Config.TargetWidth, Config.TargetHeight);
            }

            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - started;
            started = std::chrono::high_resolution_clock::now();
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Update timestep if FPS has changed
            if (Config.UpdateFPS != lastFPS) {
                lastFPS = Config.UpdateFPS;
                timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

                ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "Game");
            }

            // Setup mouse
            if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                Input::Mouse::SetScale(iw / (w - offsetX * 2.0f), ih / (h - offsetY * 2.0f));
                Input::Mouse::SetOffset(-offsetX, -offsetY);
            }

            // Skip if we are going to catch up more than 5 seconds, that is too much (May not fix what I am experiencing)
            if (lag.count() >= 5e+9) lag = std::chrono::nanoseconds(0);

            // Run Updates
            while (lag >= timeStep) {
                // Run a single update
                lag -= timeStep;
                Update();
            }

            if (Window::ShouldRenderFrame()) { // See if we should render a frame to the display
                // Take note of starting
                auto drawStart = std::chrono::high_resolution_clock::now();

                // Prep for drawing
                Graphics::Renderer::BeginDrawing();

                // Clear
                Graphics::Renderer::Clear(Graphics::TColor::Black);

                // If using, start using target
                if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                    _RenderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
                    _RenderTarget->GetTexture()->SetTextureFilter(RenderTargetFilterMode);
                    Graphics::GraphicsManager::PushTarget(_RenderTarget);
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
                        Graphics::TColor::White);
                }

                // Finish drawing
                Graphics::Renderer::EndDrawing();

                // Swap buffers
                Window::SwapBuffers();

                auto renderTime = std::chrono::high_resolution_clock::now() - drawStart;
                auto renderTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(renderTime).count();

                // Wait for the rest of the frame
                if (renderTimeMS < 1000 / Config.DrawFPS) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(renderTimeMS));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            // Reset mouse
            if (Config.MaintainResolution && _RenderTarget->IsValid()) {
                Input::Mouse::SetScale(1, 1);
                Input::Mouse::SetOffset(0, 0);
            }

            // Poll events and inputs
            Window::PollEvents();
            Input::Gamepad::PollInputs();
            Input::Mouse::PollInputs();
            Input::Keyboard::PollInputs();
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
        Config.UpdateFPS = FPS_;
        Config.DrawFPS = FPS_;
    }

    void Game::SetDrawFPS(int FPS_) {
        Config.DrawFPS = FPS_;
    }

    void Game::SetIntendedSize(TVector2 size_) {
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

    void Game::SetUpdateFPS(int FPS_) {
        Config.UpdateFPS = FPS_;
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
