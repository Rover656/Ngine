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

#include "ngine/Game.hpp"

#include "ngine/Console.hpp"
#include "ngine/audio/AudioDevice.hpp"
#include "ngine/input/Gamepad.hpp"
#include "ngine/input/Keyboard.hpp"
#include "ngine/input/Mouse.hpp"

#if defined(PLATFORM_UWP)
#include "ngine/UWP/GameApp.hpp"
#endif

namespace ngine {
    void Game::_init() {
        // Create window
        m_gameWindow = new Window(m_gameWindowCreationConfig);
        Console::notice("Game", "Created game window.");

        // Get graphics device
        auto graphicsDevice = getGraphicsDevice();

        // Create renderer.
        m_renderer = new graphics::Renderer(graphicsDevice);
        Console::notice("Game", "Created renderer.");

        // Init Gamepad
        input::Gamepad::initialize();
        Console::notice("Game", "Gamepad API has been initialized.");

        // Init audio
        Console::notice("Game", "Attempting to initialize audio device.");
        audio::AudioDevice::initialize();

        // Check if the device was created
        if (audio::AudioDevice::isReady())
            Console::notice("Game", "Audio device initialized successfully.");
        else
            Console::warn("Game", "Failed to create audio device. Audio APIs "
                                  "will be unavailable.");

        // Create render target
        // TODO: Rework this to be more robust.
        if (Config.MaintainResolution) {
            m_renderTarget = new graphics::RenderTarget(
                graphicsDevice, Config.TargetWidth, Config.TargetHeight);
        }

        // Create resource manager
        Console::notice("Game", "Creating the game resource manager.");
        m_resourceManager = new filesystem::ResourceManager(graphicsDevice);

        // DEV: If anything new is added to the Game class, its initialization
        // should occur here (Before OnInit())

        // Initialize
        initialize();

        // Init Timing
        m_lag = std::chrono::nanoseconds(0);
        m_started = std::chrono::high_resolution_clock::now();
        m_timestep = std::chrono::milliseconds(
            int(1000.0f / float(Config.UpdatesPerSecond)));
    }

    void Game::_runDraw() {
        // Poll events
        m_gameWindow->pollEvents();

        // Don't render if suspended
        if (m_suspended) {
            // Do nothing this frame.
            std::this_thread::sleep_for(m_timestep);
            return;
        }

        // Make our window current so we can render to it.
        m_gameWindow->makeCurrent();

        // Get graphics device
        auto graphicsDevice = getGraphicsDevice();

        // Window/Game Size variables
        const auto w = static_cast<float>(m_gameWindow->getWidth());
        const auto h = static_cast<float>(m_gameWindow->getHeight());
        const auto iw = static_cast<float>(Config.TargetWidth);
        const auto ih = static_cast<float>(Config.TargetHeight);
        const auto scale = std::min(w / iw, h / ih);
        const auto offsetX = (w - iw * scale) * 0.5f;
        const auto offsetY = (h - ih * scale) * 0.5f;

        // TODO: Clean this mess
        if (Config.MaintainResolution && m_renderTarget == nullptr) {
            m_renderTarget = new graphics::RenderTarget(
                graphicsDevice, Config.TargetWidth, Config.TargetHeight);
        } else if (Config.MaintainResolution &&
                   (!m_renderTarget->isValid() ||
                    (m_renderTarget->Width != Config.TargetWidth ||
                     m_renderTarget->Height != Config.TargetHeight))) {
            delete m_renderTarget;
            m_renderTarget = new graphics::RenderTarget(
                graphicsDevice, Config.TargetWidth, Config.TargetHeight);
        }

        if (Config.MaintainResolution && m_renderTarget != nullptr)
            m_renderTarget->getTexture()->setTextureFilter(
                RenderTargetFilterMode);

        // Get the time that we begin (for timing)
        auto frameBegin = std::chrono::high_resolution_clock::now();

        // Setup mouse translation
        if (Config.MaintainResolution && m_renderTarget->isValid()) {
            m_virtualMouseContext.XScale = iw / (w - offsetX * 2.0f);
            m_virtualMouseContext.YScale = ih / (h - offsetY * 2.0f);
            m_virtualMouseContext.XOffset = offsetX;
            m_virtualMouseContext.YOffset = offsetY;
        }

        // Only render if visible
        if (m_gameWindow->isVisible()) {
            // Mouse
            const auto mouse = m_gameWindow->getMouse();

            if (Config.MaintainResolution && m_renderTarget->isValid()) {
                mouse->_setContext(&m_virtualMouseContext);
            }

            // If using, start render target
            if (Config.MaintainResolution && m_renderTarget->isValid()) {
                // Clear the main framebuffer (for black bars)
                graphicsDevice->clear(graphics::Color::Black);

                // Enable our render target
                graphicsDevice->pushTarget(m_renderTarget);
            }

            // Clear with clear color
            graphicsDevice->clear(ClearColor);

            // Render scene
            if (m_currentScene != nullptr) {
                m_currentScene->renderScene(m_renderer);
            }

            // If using a target, draw target
            if (Config.MaintainResolution && m_renderTarget->isValid()) {
                // Remove target from stack
                graphicsDevice->popTarget();

                // We use Screen coordinates to put our internal framebuffer up,
                // but any'll do.
                m_renderer->setCoordinateSystem(
                    graphics::CoordinateSystem::Screen);

                // Draw render target.
                m_renderTarget->getTexture()->draw(
                    m_renderer, {offsetX, offsetY, iw * scale, ih * scale},
                    {0, 0, iw, ih * -1}, graphics::Color::White, {0, 0});
            }

            // Render final buffers
            m_renderer->renderBatch();

            // Swap buffers
            m_gameWindow->swapBuffers();

            if (Config.MaintainResolution && m_renderTarget->isValid()) {
//                mouse->_setContext(nullptr);
            }
        }

        // FPS limiter
        if (!m_gameWindow->getVSyncEnabled()) {
            auto frameTime =
                std::chrono::high_resolution_clock::now() - frameBegin;
            auto frameTimeMS =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    frameTime);
            auto timestep = std::chrono::milliseconds(
                (int)((float)1e+3f / (float)Config.FPSCap));

            // If we were too quick.
            if (frameTimeMS < timestep) {
                std::this_thread::sleep_for(timestep - frameTimeMS);
            }
        }
    }

    void Game::_runUpdate() {
        // Start timing here so if updates don't happen, frame skips don't
        // either.
        auto now = std::chrono::high_resolution_clock::now();
        auto deltaTime = now - m_started;
        m_started = now;

        // Check if we are/should be suspended
        if (!m_gameWindow->isVisible() && !Config.RunWhileHidden) {
            // If told to quit, quit
            if (!m_running)
                return;

            // Suspend if we haven't
            if (!m_suspended) {
                EventDispatcher::fire(SuspendEvent(this));
                m_suspended = true;
            }

            // Do nothing this frame.
            std::this_thread::sleep_for(m_timestep);
        } else {
            // Resume if we had suspended
            if (m_suspended) {
                EventDispatcher::fire(ResumeEvent(this));
                m_suspended = false;
            }

            // Update timestep if FPS has changed
            if (m_timestep.count() !=
                int(1000.0f / (float)Config.UpdatesPerSecond)) {
                m_timestep = std::chrono::milliseconds(
                    int(1000.0f / (float)Config.UpdatesPerSecond));
                Console::notice(
                    "Game", "Timestep updated to match updates per second.");
            }

            // Increment lag
            m_lag +=
                std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

            // Get input managers
            auto mouse = getMouse();
            auto keyboard = getKeyboard();

            // Setup mouse translation
            if (Config.MaintainResolution && m_renderTarget->isValid()) {
                // Assume render thread kept the context up to date.
                if (Config.MaintainResolution && m_renderTarget->isValid()) {
                    mouse->_setContext(&m_virtualMouseContext);
                }
            }

            // Check if we are behind by 15+ frames.
            if (m_lag >= m_timestep * 15) {
                Console::warn(
                    "Game",
                    "Running behind by %i frame(s) or %d ms, skipping...",
                    (int)(m_lag / m_timestep),
                    std::chrono::duration_cast<std::chrono::milliseconds>(m_lag)
                        .count());
                m_lag = std::chrono::nanoseconds(0);
            }

            // Poll inputs if we'd update
            if (m_lag >= m_timestep) {
                input::Gamepad::pollInputs();
                mouse->pollInputs();
                keyboard->pollInputs();
            }

            // Run required updates
            while (m_lag >= m_timestep) {
                // Run a single update
                m_lag -= m_timestep;

                // Update the current scene.
                if (m_currentScene != nullptr) {
                    m_currentScene->update();
                    m_currentScene->updateUI();
                }

                // AudioDevice update
                audio::AudioDevice::update();

                // If we need to quit, don't run any more frames
                if (!m_running)
                    return;
            }

            if (Config.MaintainResolution && m_renderTarget->isValid()) {
//                mouse->_setContext(nullptr);
            }

            // If we need to quit, don't render
            if (!m_running)
                return;

            // Release thread for others.
            std::this_thread::sleep_for(std::chrono::milliseconds(0));
        }
    }

    void Game::_updateThread() {
        while (m_running) {
            _runUpdate();
        }
    }

    void Game::_cleanup() {
        // Delete render target now so that it doesnt try after GL is gone.
        m_renderTarget = nullptr;

        // Call the suspend event.
        EventDispatcher::fire(SuspendEvent(this));

        // Cleanup
        cleanup();

        // Delete resource manager
        delete m_resourceManager;

        // Close audio
        audio::AudioDevice::close();
        Console::notice("Game", "Closed audio device.");

        // Delete render target
        delete m_renderTarget;
        m_renderTarget = nullptr;

        // Delete renderer
        delete m_renderer;
        m_renderer = nullptr;
        Console::notice("Game", "Deleted renderer.");

        // Close window
        delete m_gameWindow;
        m_gameWindow = nullptr;
        Console::notice("Game", "Deleted game window.");
    }

    Game::Game(WindowConfig windowConfig, const GameConfig &config)
        : m_gameWindowCreationConfig(std::move(windowConfig)), Config(config),
          m_virtualViewport(0, 0, (float)config.TargetWidth,
                            (float)config.TargetHeight) {}

    void Game::initialize() {}

    void Game::cleanup() {}

    Window *Game::getGameWindow() const { return m_gameWindow; }

    input::Mouse *Game::getMouse() const {
        return m_gameWindow ? m_gameWindow->getMouse() : nullptr;
    }

    input::Keyboard *Game::getKeyboard() const {
        return m_gameWindow ? m_gameWindow->getKeyboard() : nullptr;
    }

    graphics::GraphicsDevice *Game::getGraphicsDevice() {
        return m_gameWindow->getGraphicsDevice();
    }

    Vector2 Game::getDefaultWindowDimensions() const {
        return {(float)m_gameWindow->getWidth(),
                (float)m_gameWindow->getHeight()};
    }

    Vector2 Game::getDimensions() const {
        return {(float)Config.TargetWidth, (float)Config.TargetHeight};
    }

    const graphics::Viewport *Game::getViewport() {
        if (Config.MaintainResolution)
            return &m_virtualViewport;
        else return m_gameWindow->getWindowViewport();
    }

    int Game::getTargetFPS() const { return Config.UpdatesPerSecond; }

    void Game::run() {
#if defined(PLATFORM_UWP)
        throw std::runtime_error("For the UWP platform, the Run() method must "
                                 "not be used! Use UWP::GameApp instead.");
#else
        // Init game.
        _init();

        // Mark as running
        m_running = true;

        // Start update thread
        std::thread updateThread(&Game::_updateThread, this);

        // This checks the game should still run
        while (!m_gameWindow->shouldClose() && m_running) {
            // Draw frame
            _runDraw();
        }

        // No longer running
        m_running = false;

        // Join update thread back into main
        updateThread.join();

        // Clean up
        _cleanup();
#endif
    }

    bool Game::isRunning() { return m_running; }

    void Game::quit() { m_running = false; }

    void Game::setFPS(int FPS) { Config.UpdatesPerSecond = FPS; }

    void Game::setIntendedSize(Vector2 size) {
        Config.TargetWidth = (int)size.X;
        Config.TargetHeight = (int)size.Y;
    }

    void Game::setIsRunning(bool running) { m_running = running; }

    void Game::setScene(Scene *scene) {
        // Cleanup current scene.
        if (m_currentScene != nullptr)
            m_currentScene->cleanup();

        auto old = m_currentScene;
        m_currentScene = scene;

        // Fire changed event
        EventDispatcher::fire(SceneChangedEvent(this, old, scene));

        // Initialize new scene.
        if (m_currentScene != nullptr)
            m_currentScene->initialize(this);

        Console::notice("Game", "A new scene has been loaded.");
    }

    filesystem::ResourceManager *Game::getResourceManager() const {
        return m_resourceManager;
    }
} // namespace ngine