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

namespace Ngine {
    namespace Core {
        class NEAPI Game {
            // Private Fields

            /*
             * The current FPS
             */
            int _FPS;

            //TODO: Decide to store width or height or leave it to raylib

        public:
            // Public Constructor(s)

            /*
             * Create a new Game
             */
            Game(int width_, int height_, int FPS_, std::string title_);

            // Public Methods

            /*
             * Draw a frame
             */
            virtual void Draw();

            /*
             * Run the game
             */
            void Run();

            /*
             * Set the target FPS
             * The FPS determines how many times Update is called per second
             */
            void SetFPS(int FPS_);

            /*
             * Update, this handles logic once per frame
             */
            virtual void Update();
        };
    }
}

#endif // GAME_H
