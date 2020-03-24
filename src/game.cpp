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

#include "ngine/game.hpp"

namespace ngine {
    void Game::_updateThread() {
        while (m_running) {
            update();
        }
    }

    Game::Game(WindowConfig windowConfig) {
        m_windowConfig = windowConfig;
    }

    Game::~Game() {

    }

    void Game::run() {
        // Start update thread
        m_updateThread = std::thread(&Game::_updateThread, this);

        // Init
        init();

        // Run draw thread
        while (m_running) {
            m_window->pollEvents();
            m_window->swapBuffers();

            m_running = !m_window->pendingClose();
        }

        // Finish update thread
        m_updateThread.join();

        // Cleanup
        cleanup();
    }

    void Game::init() {
        // Create window
        m_window = new Window(m_windowConfig);
    }

    void Game::cleanup() {
        // Delete window
        delete m_window;
    }

    void Game::draw() {

    }

    void Game::update() {

    }
}