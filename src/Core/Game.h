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

#ifndef GAME_H
#define GAME_H

// Include ngine
#include "../ngine.h"

#include "../Graphics/Drawing.h"

namespace Ngine {
    namespace Core {
        enum GameConfig {
            None = 0,
            FULLSCREEN = 2,
            RESIZEABLE_WINDOW = 4,
            FRAMELESS_WINDOW = 8,
            TRANSPARENT_WINDOW = 16,
            HIDDEN_WINDOW = 128,
            MSAA_4X = 32,
            VSYNC = 64
        };

        class NEAPI Game {
            // Private Fields

            /*
             * The currently loaded scene
             */
            BaseScene* _CurrentScene = nullptr;

            /*
             * The target draw FPS
             */
            int _DrawFPS = 0;

            /*
             * The target update FPS
             */
            int _UpdateFPS = 0;

        public:
            // Public Constructor(s)

            /*
             * Create a new Game
             */
            Game(int width_, int height_, int FPS_, const std::string &title_, GameConfig config_ = None);

            /*
             * Create a new Game (Advanced)
             */
            Game(int width_, int height_, int drawFPS_, int updateFPS_, const std::string &title_, GameConfig config_ = None);

            // Public Methods

            /*
             * Draw a frame.
             * This expects to be called after BeginDrawing
             */
            virtual void Draw();

            /*
             * Get the target FPS.
             * If using advanced game will return update FPS
             */
            int GetFPS() const;

            /*
             * Get the target draw FPS.
             */
            int GetDrawFPS() const;

            /*
             * Get the target update FPS.
             */
            int GetUpdateFPS() const;

            /*
             * Start the game loop
             */
            void Run();

            /*
             * Set the target FPS.
             * If using advanced game, this will set both update and draw FPS
             */
            void SetFPS(int FPS_);

            /*
             * Set the target draw FPS
             */
            void SetDrawFPS(int FPS_);

            /*
             * Set the current scene
             */
            void SetScene(BaseScene* scene_);

            /*
             * Set the target update FPS
             */
            void SetUpdateFPS(int FPS_);

            /*
             * Update logic
             */
            virtual void Update();

        protected:
            // Protected Methods

            /*
             * Clear the game background
             */
            void Clear();
        };
    }
}

#endif //GAME_H
