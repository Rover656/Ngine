/**********************************************************************************************
 *
 *   Ngine - A game framework.
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

#include "ngine/console.hpp"
#include "ngine/window.hpp"

namespace ngine {
    Game::Game(WindowDesc windowConfig) {
        m_windowConfig = windowConfig;
    }

    Game::~Game() {

    }

    IWindow *Game::getWindow() {
        return m_window;
    }

    graphics::IGraphicsDevice *Game::getGraphicsDevice() {
        return m_window->getGraphicsDevice();
    }

    void Game::run() {
        // Start update thread
        m_updateThread = std::thread(&Game::_updateThread, this);

        // Init
        init();

        // Run draw thread
        while (m_running) {
            _performDraw();
        }

        // Finish update thread
        m_updateThread.join();

        // Cleanup
        cleanup();
    }

    void Game::init() {
        // Create window
        m_window = IWindow::createWindow(m_windowConfig);
    }

    void Game::cleanup() {
        // Delete window
        delete m_window;
    }

    void Game::draw() {

    }

    void Game::update() {

    }

    void Game::_updateThread() {
        while (m_running) {
            _performUpdate();
        }
    }

    void Game::_performDraw() {
        // Poll window events
        m_window->pollEvents();

        // Clear the screen
        //m_window->getGraphicsDevice()->clear(graphics::Color::Blue);

        // Draw
        //draw();

        // Push to screen
        //getGraphicsDevice()->present();

        // Determine if we should close
        m_running = !m_window->shouldClose();

        // TODO: Frame limiter
    }

    void Game::_performUpdate() {
        // TODO: "More advanced"
        update();
    }
}