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

// Include raylib
#include "../3rd-party/raylib/src/raylib.h"

// Threading
#include <thread>

namespace Ngine {
    namespace Core {
        // Public Constructor(s)

        Game::Game(int width_, int height_, int FPS_, std::string title_) {
            #if !defined(PLATFORM_UWP)

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

        void Game::Draw() { }

        void Game::Run() {
            #if !defined(PLATFORM_UWP)

            // Standard game loop

            std::chrono::nanoseconds lag(0);
            auto started = std::chrono::high_resolution_clock::now();

            int lastFPS = _FPS;
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
                BeginDrawing();

                // Clear the background
                ClearBackground(BLACK);

                // Draw
                Draw();

                // Finish drawing
                EndDrawing();

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

        void Game::Update() { }
    }
}
