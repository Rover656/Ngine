/**********************************************************************************************
 *
 *   Ngine - A 2D game engine.
 *
 *   Copyright 2020 NerdThings (Reece Mackie)
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 **********************************************************************************************/

#ifndef NGINE_GAME_HPP
#define NGINE_GAME_HPP

#include "ngine/config.hpp"

#include "app.hpp"
#include "window.hpp"

#include <thread>

namespace ngine {
    class NEAPI Game : public App {
    public:
        /**
         * Create a new game.
         *
         * @param windowConfig Window creation config.
         */
        Game(WindowConfig windowConfig);

        ~Game();

        /**
         * Get the game window.
         */
        Window *getWindow();

        /**
         * Get the window graphics device.
         */
        graphics::GraphicsDevice *getGraphicsDevice();

        /**
         * Run the game.
         * This will become the "main thread".
         */
        void run() override final;

        void init() override;

        void cleanup() override;

        void draw() override;// final;

        void update() override final;

    private:
        /**
         * The update thread.
         */
        std::thread m_updateThread;

        /**
         * Whether or not the game is running.
         */
        bool m_running = true;

        /**
         * The game window.
         */
        Window *m_window = nullptr;

        /**
         * Config to create the game window with.
         */
        WindowConfig m_windowConfig;

        /**
         * Internal update thread.
         */
        void _updateThread();
    };
}

#endif //NGINE_GAME_HPP
