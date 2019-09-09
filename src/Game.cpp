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

#include "Audio/AudioManager.h"
#include "Graphics/GraphicsManager.h"
#include "Input/Gamepad.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Filesystem/Resources.h"
#include "Window.h"

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

    Game::Game(const int width_, const int height_, const int FPS_, const std::string &title_, int config_)
        : Game(width_, height_, FPS_, FPS_, title_, config_) {}

    Game::Game(const int width_, const int height_, const int drawFPS_, const int updateFPS_,
               const std::string &title_, int config_)
        : Game(width_, height_, width_, height_, drawFPS_, updateFPS_, title_, config_) {}

    Game::Game(int windowWidth_, int windowHeight_, int targetWidth_, int targetHeight_, int drawFPS_, int updateFPS_,
               const std::string &title_, int config_) {
        // Check a game is not present
        if (CurrentGame != nullptr)
            throw std::runtime_error("Cannot create a second game before deleting the first!");
        CurrentGame = this;

        // Save config
        _Config = config_;

        // Apply config
        //WindowManager::ApplyConfig(_Config);
        //ConsoleMessage("Window config has been applied.", "NOTICE", "GAME");

        // Set intended dimensions
        _IntendedHeight = targetHeight_;
        _IntendedWidth = targetWidth_;

        // Set default window dimensions
        _WindowHeight = windowHeight_;
        _WindowWidth = windowWidth_;

#if !defined(PLATFORM_UWP)
        // Initialize window
        Window::Init(_WindowWidth, _WindowHeight, title_);
        ConsoleMessage("Window has been initialized.", "NOTICE", "Game");
#endif

        // Set Target FPS
        // TODO: We need to implement this now... for now these are just stored numbers
        SetDrawFPS(drawFPS_);
        SetUpdateFPS(updateFPS_);

        // Register default events
        OnUpdate.Bind(Audio::AudioManager::Update);
        ConsoleMessage("Engine events have been registered.", "NOTICE", "Game");
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

    int Game::GetConfig()
    {
        return _Config;
    }

    int Game::GetFPS() const {
        return _UpdateFPS;
    }

    TVector2 Game::GetDefaultWindowDimensions() const {
        return { (float)_WindowWidth, (float)_WindowHeight };
    }

    TVector2 Game::GetDimensions() const {
        return { (float)_IntendedWidth, (float)_IntendedHeight };
    }

    int Game::GetDrawFPS() const {
        return _DrawFPS;
    }

    int Game::GetUpdateFPS() const {
        return _UpdateFPS;
    }

    bool Game::IsRunning() {
        return _Running;
    }

    void Game::Quit() {
        _Running = false;
    }

    void Game::Run() {
        // Create render target
        if (_Config & MAINTAIN_DIMENSIONS) {
            _RenderTarget = std::make_shared<Graphics::TRenderTarget>(_IntendedWidth, _IntendedHeight);
        }

        // Timing
        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();

        auto lastFPS = _UpdateFPS;
        auto timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

        // Init audio
        ConsoleMessage("Attempting to initialize audio device.", "NOTICE", "Game");
        Audio::AudioManager::InitDevice();

        // Check if the device was created
        if (Audio::AudioManager::IsReady())
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
            const auto iw = static_cast<float>(_IntendedWidth);
            const auto ih = static_cast<float>(_IntendedHeight);
            const auto scale = std::min(w / iw, h / ih);
            const auto offsetX = (w - iw * scale) * 0.5;
            const auto offsetY = (h - ih * scale) * 0.5;

            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - started;
            started = std::chrono::high_resolution_clock::now();
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Update timestep if FPS has changed
            if (_UpdateFPS != lastFPS) {
                lastFPS = _UpdateFPS;
                timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

                ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "Game");
            }

            // Setup mouse
            if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget->IsValid()) {
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

            if (Window::ShouldRenderFrame()) { // See if we should render a frame to the displau
                // Prep for drawing
                Graphics::Renderer::BeginDrawing();

                // Clear
                Graphics::Renderer::Clear(Graphics::TColor::Black);

                // If using, start using target
                if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget->IsValid()) {
                    _RenderTarget->GetTexture()->SetTextureWrap(Graphics::WRAP_CLAMP);
                    _RenderTarget->GetTexture()->SetTextureFilter(RenderTargetFilterMode);
                    Graphics::GraphicsManager::PushTarget(_RenderTarget);
                }

                // Clear the background
                Clear();

                // Draw
                Draw();

                // If using a target, draw target
                if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget->IsValid()) {
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
            }

            // Reset mouse
            if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget->IsValid()) {
                Input::Mouse::SetScale(1, 1);
                Input::Mouse::SetOffset(0, 0);
            }

            // Poll events and inputs
            Window::PollEvents();
            Input::Gamepad::PollInputs();
            Input::Mouse::PollInputs();
            Input::Keyboard::PollInputs();

#if defined(PLATFORM_DESKTOP)
            // Release thread to CPU (Stops weird idle cpu usage and fps drops)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
        }

        // Delete render target now so that it doesnt try after GL is gone.
        _RenderTarget = nullptr;

#if !defined(PLATFORM_UWP) // UWP handles this elsewhere because this is ignored
        // Delete loaded resources
        Filesystem::Resources::DeleteAll();

        // Close audio
        ConsoleMessage("Closing audio device.", "NOTICE", "Game");
        Audio::AudioManager::CloseDevice();

        // Close window
        ConsoleMessage("Closing window.", "NOTICE", "Game");
        Window::Cleanup();

        ConsoleMessage("Game successfully stopped.", "NOTICE", "Game");
#endif
    }

    void Game::SetFPS(int FPS_) {
        _UpdateFPS = FPS_;
        _DrawFPS = FPS_;
        //WindowManager::SetTargetFPS(FPS_);
    }

    void Game::SetDrawFPS(int FPS_) {
        _DrawFPS = FPS_;
        //WindowManager::SetTargetFPS(FPS_);
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
        _UpdateFPS = FPS_;
    }

    void Game::Update() {
        // Run update events
        OnUpdate({});

        if (_CurrentScene != nullptr) {
            _CurrentScene->Update();
        }
    }
}
