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

#include "../Audio/AudioManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "Resources.h"
#include "WindowManager.h"
#include "../../../../../../../Dev/msys64/mingw64/include/c++/8.3.0/chrono"

namespace NerdThings::Ngine::Core {
    // Public Constructor(s)

    Game::Game(const int width_, const int height_, const int FPS_, const std::string &title_, int config_)
        : Game(width_, height_, FPS_, FPS_, title_, config_) {}

    Game::Game(const int width_, const int height_, const int drawFPS_, const int updateFPS_,
               const std::string &title_, int config_)
        : Game(width_, height_, width_, height_, drawFPS_, updateFPS_, title_, config_) {}

    Game::Game(int windowWidth_, int windowHeight_, int targetWidth_, int targetHeight_, int drawFPS_, int updateFPS_,
               const std::string &title_, int config_) {
        #if !defined(PLATFORM_UWP)

        // Save config
        _Config = config_;

        // Apply raylib config
        WindowManager::ApplyConfig(_Config);
        ConsoleMessage("Window config has been applied.", "NOTICE", "GAME");

        // Set intended dimensions
        _IntendedHeight = targetHeight_;
        _IntendedWidth = targetWidth_;

        // Initialize raylib's window
        WindowManager::Init(windowWidth_, windowHeight_, title_);
        ConsoleMessage("Window has been initialized.", "NOTICE", "GAME");

        // Set Target FPS
        SetDrawFPS(drawFPS_);
        SetUpdateFPS(updateFPS_);

        // Register default events
        OnRun.Bind(Input::Mouse::OnGameRun);
        OnUpdate.Bind(Input::Mouse::OnGameUpdate);
        OnUpdate.Bind(Audio::AudioManager::Update);
        ConsoleMessage("Engine events have been registered.", "NOTICE", "GAME");

        // By default, disable exit key
        Input::Keyboard::SetExitKey(KEY_NONE);

        #else

        // TODO: UWP Support

        #endif
    }

    // Public Methods

    void Game::Draw() {
        if (_CurrentScene != nullptr) {
            OnDraw({});
            _CurrentScene->Draw();
        }
    }

    int Game::GetFPS() const {
        return _UpdateFPS;
    }

    Math::TVector2 Game::GetDimensions() const {
        return { (float)_IntendedWidth, (float)_IntendedHeight };
    }

    int Game::GetDrawFPS() const {
        return _DrawFPS;
    }

    int Game::GetUpdateFPS() const {
        return _UpdateFPS;
    }

    void Game::Quit() {
        _Running = false;
    }

    void Game::Run() {
        #if !defined(PLATFORM_UWP)

        // Create render target
        if (_Config & MAINTAIN_DIMENSIONS) {
            _RenderTarget = Graphics::TRenderTarget(_IntendedWidth, _IntendedHeight);
        }

        // Timing
        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();

        auto lastFPS = _UpdateFPS;
        auto timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

        // Init audio
        ConsoleMessage("Attempting to initialize audio device.", "NOTICE", "GAME");
        Audio::AudioManager::InitDevice();

        // Check if the device was created
        if (Audio::AudioManager::IsReady()) {
            ConsoleMessage("Audio device initialized successfully..", "NOTICE", "GAME");
        } else {
            ConsoleMessage("Failed to create audio device, audio will be unavailable.", "WARNING", "GAME");
        }

        // Invoke OnRun
        OnRun({});

        _Running = true;

        while (!WindowManager::ShouldClose() && _Running) {
            // Window/Game Size variables
            const auto w = static_cast<float>(WindowManager::GetWindowWidth());
            const auto h = static_cast<float>(WindowManager::GetWindowHeight());
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

                ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "GAME");
            }

            // Setup mouse
            if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget.ID > 0) {
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

            // Prep for drawing
            Graphics::Drawing::BeginDrawing();

            // Clear
            Graphics::Drawing::Clear(Graphics::TColor::Black);

            // If using, start using target
            if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget.ID > 0) {
                _RenderTarget.Texture.SetTextureWrap(WRAP_CLAMP);
                _RenderTarget.Texture.SetTextureFilter(RenderTargetFilterMode);
                Graphics::GraphicsManager::PushTarget(&_RenderTarget);
            }

            // Clear the background
            Clear();

            // Draw
            Draw();

            // If using a target, draw target
            if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget.ID > 0) {
                auto popped = false;
                Graphics::GraphicsManager::PopTarget(popped);

                Graphics::Drawing::DrawTexture(&_RenderTarget.Texture,
                                               {
                                                   (w - iw * scale) * 0.5f,
                                                   (h - ih * scale) * 0.5f,
                                                   iw * scale,
                                                   ih * scale
                                               },
                                               {
                                                   0,
                                                   0,
                                                   static_cast<float>(_RenderTarget.Texture.Width),
                                                   static_cast<float>(-_RenderTarget.Texture.Height)
                                               },
                                               Graphics::TColor::White);
            }

            // Reset mouse
            if (_Config & MAINTAIN_DIMENSIONS && _RenderTarget.ID > 0) {
                Input::Mouse::SetScale(1, 1);
                Input::Mouse::SetOffset(0, 0);
            }

            // Finish drawing
            Graphics::Drawing::EndDrawing();

            // Release thread to CPU (Stops weird idle cpu usage and fps drops)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // Delete loaded resources
        Resources::DeleteAll();

        // Close audio
        ConsoleMessage("Closing audio device.", "NOTICE", "GAME");
        Audio::AudioManager::CloseDevice();

        // Close window
        ConsoleMessage("Closing window.", "NOTICE", "GAME");
        WindowManager::Close();

        ConsoleMessage("Game successfully shut down.", "NOTICE", "GAME");

        #else

        // TODO: UWP Support

        #endif
    }

    void Game::SetFPS(int FPS_) {
        _UpdateFPS = FPS_;
        _DrawFPS = FPS_;
        WindowManager::SetTargetFPS(FPS_);
    }

    void Game::SetDrawFPS(int FPS_) {
        _DrawFPS = FPS_;
        WindowManager::SetTargetFPS(FPS_);
    }

    void Game::SetScene(Scene *scene_) {
        if (_CurrentScene != nullptr)
            _CurrentScene->OnUnLoad({this});

        _CurrentScene = scene_;

        if (_CurrentScene != nullptr)
            _CurrentScene->OnLoad({this});

        ConsoleMessage("A new scene has been loaded.", "NOTICE", "GAME");
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

    // Protected Methods

    void Game::Clear() const {
        Graphics::Drawing::Clear(ClearColor);
    }
}
