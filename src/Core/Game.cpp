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

#include "../Input/Mouse.h"

namespace Ngine::Core {
    // Public Constructor(s)

    Game::Game(const int width_, const int height_, const int FPS_, const std::string &title_, int config_)
        : Game(width_, height_, FPS_, FPS_, title_, config_) {}

    Game::Game(const int width_, const int height_, const int drawFPS_, const int updateFPS_,
               const std::string &title_, int config_) {
        #if !defined(PLATFORM_UWP)

        // Apply config
        SetConfigFlags(config_);

        // Initialize raylib's window
        InitWindow(width_, height_, title_.c_str());

        // Set Target FPS
        SetDrawFPS(drawFPS_);
        SetUpdateFPS(updateFPS_);

        // Register default events
        OnRun.Bind(Input::Mouse::OnGameRun);
        OnUpdate.Bind(Input::Mouse::OnGameUpdate);

        #else

        // TODO: UWP Support

        #endif
    }

    // Public Methods

    void Game::Draw() {
        // OnDraw event
        OnDraw.Invoke(EventArgs());

        // TODO: Some form of draw scaling system (keeping a target ratio or dimensions)
        if (_CurrentScene != nullptr) {
            const auto _2dScene = dynamic_cast<Scene2D*>(_CurrentScene);

            if (_2dScene != nullptr) {
                _2dScene->GetActiveCamera()->BeginCamera();
            } // TODO: 3D?

            _CurrentScene->Draw();

            if (_2dScene != nullptr) {
                TCamera2D::EndCamera();
            } // TODO: 3D?
        }
    }

    int Game::GetFPS() const {
        return _UpdateFPS;
    }

    int Game::GetDrawFPS() const {
        return _DrawFPS;
    }

    int Game::GetUpdateFPS() const {
        return _UpdateFPS;
    }

    void Game::Run() {
        #if !defined(PLATFORM_UWP)

        // Standard game loop

        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();

        auto lastFPS = _UpdateFPS;
        auto timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

        // Invoke OnRun
        OnRun.Invoke(EventArgs());

        while (!WindowShouldClose()) {
            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - started;
            started = std::chrono::high_resolution_clock::now();
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Update timestep if FPS has changed
            if (_UpdateFPS != lastFPS) {
                lastFPS = _UpdateFPS;
                timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));
            }

            // Run Updates
            while (lag >= timeStep) {
                // Run a single update
                lag -= timeStep;
                Update();
            }

            // Prep for drawing
            Graphics::Drawing::BeginDrawing();

            // Clear the background
            Clear();

            // Draw
            Draw();

            // Finish drawing
            Graphics::Drawing::EndDrawing();

            // Release thread to CPU (Stops wierd idle cpu usage)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        CloseWindow();

        #else

        // TODO: UWP Support

        #endif
    }

    void Game::SetFPS(int FPS_) {
        _UpdateFPS = FPS_;
        _DrawFPS = FPS_;
        SetTargetFPS(FPS_);
    }

    void Game::SetDrawFPS(int FPS_) {
        _DrawFPS = FPS_;
        SetTargetFPS(FPS_);
    }

    void Game::SetScene(BaseScene *scene_) {
        _CurrentScene = scene_;
        // TODO: Load and unload events
    }

    void Game::SetUpdateFPS(int FPS_) {
        _UpdateFPS = FPS_;
    }

    void Game::Update() {
        // Run update events
        OnUpdate.Invoke(EventArgs());

        if (_CurrentScene != nullptr) {
            // We also set the camera here so that mouse translations etc. work

            const auto _2dScene = dynamic_cast<Scene2D*>(_CurrentScene);

            if (_2dScene != nullptr) {
                _2dScene->GetActiveCamera()->BeginCamera();
            } // TODO: 3D?

            _CurrentScene->Draw();

            if (_2dScene != nullptr) {
                TCamera2D::EndCamera();
            } // TODO: 3D?
        }
    }

    // Protected Methods

    void Game::Clear() const {
        Graphics::Drawing::Clear(ClearColor);
    }
}
