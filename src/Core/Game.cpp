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

namespace Ngine::Core {
    // Public Constructor(s)

    Game::Game(int width_, int height_, int FPS_, std::string title_) {
        #if !defined(PLATFORM_UWP)

        // TODO: Config flags support

        // Initialize raylib's window
        InitWindow(width_, height_, title_.c_str());

        // Set Target FPS
        _FPS = FPS_;
        SetTargetFPS(FPS_);

        #else

        // TODO: UWP Support

        #endif
    }

    // Public Methods

    void Game::Draw() {
        // TODO: Some form of draw scaling system (keeping a target ratio or dimensions)
        if (_CurrentScene != nullptr) {
            const auto _2dScene = dynamic_cast<Scene2D*>(_CurrentScene);

            if (_2dScene != nullptr) {
                Graphics::Drawing::BeginCamera2D(_2dScene->GetActiveCamera());
            } // TODO: 3D?

            _CurrentScene->Draw();

            if (_2dScene != nullptr) {
                Graphics::Drawing::EndCamera2D();
            } // TODO: 3D?
        }
    }

    int Game::GetFPS() const {
        return _FPS;
    }

    void Game::Run() {
        #if !defined(PLATFORM_UWP)

        // Standard game loop

        std::chrono::nanoseconds lag(0);
        auto started = std::chrono::high_resolution_clock::now();

        auto lastFPS = _FPS;
        auto timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

        while (!WindowShouldClose()) {
            // Get the time since the last frame
            auto deltaTime = std::chrono::high_resolution_clock::now() - started;
            started = std::chrono::high_resolution_clock::now();
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Update timestep if FPS has changed
            if (_FPS != lastFPS) {
                lastFPS = _FPS;
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
        _FPS = FPS_;
        SetTargetFPS(FPS_);
    }

    void Game::Update() {
        if (_CurrentScene != nullptr) {
            // We also set the camera here so that mouse translations etc. work

            const auto _2dScene = dynamic_cast<Scene2D*>(_CurrentScene);

            if (_2dScene != nullptr) {
                Graphics::Drawing::BeginCamera2D(_2dScene->GetActiveCamera());
            } // TODO: 3D?

            _CurrentScene->Draw();

            if (_2dScene != nullptr) {
                Graphics::Drawing::EndCamera2D();
            } // TODO: 3D?
        }
    }

    // Protected Methods

    void Game::Clear() {
        //TODO: Configurable
        Graphics::Drawing::Clear(Black);
    }
}
