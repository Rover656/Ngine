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
        class NEAPI Game {
            // Private Fields

            /*
             * The currently loaded scene
             */
            BaseScene* _CurrentScene = nullptr;

            /*
             * The target FPS
             */
            int _FPS = 0;

        public:
            // Public Constructor(s)

            /*
             * Create a new Game
             */
            Game(int width_, int height_, int FPS_, std::string title_);

            // Public Methods

            /*
             * Draw a frame.
             * This expects to be called after BeginDrawing
             */
            virtual void Draw();

            /*
             * Get the target FPS
             */
            int GetFPS() const;

            /*
             * Start the game loop
             */
            void Run();

            /*
             * Set the target FPS
             */
            void SetFPS(int FPS_);

            /*
             * Set the current scene
             */
            void SetScene(BaseScene* scene_);

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
