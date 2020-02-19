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

#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"

#include "graphics/Color.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/RenderTarget.hpp"
#include "filesystem/ResourceManager.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Scene.hpp"
#include "Window.hpp"

namespace ngine {
#if defined(PLATFORM_UWP)
    namespace UWP {
        ref class GameApp;
    }
#endif

    /**
     * Game configuration structure.
     * This details exactly how the game should be configured.
     */
    struct GameConfig {
        /**
         * Number of times to update
         */
        int UpdatesPerSecond = 60;

        /**
         * The render cap (-1 for no cap).
         */
        int FPSCap = 60;

        /**
         * Target game height (virtual resolution).
         */
        int TargetHeight = 0;

        /**
         * Maintain game virtual resolution.
         */
        bool MaintainResolution = false;

        /**
         * Run the game while not visible.
         */
        bool RunWhileHidden = true;

        /**
         * Target game width (virtual resolution).
         */
        int TargetWidth = 0;
    };

    /**
     * The game contains every scene in your game.
     */
    class NEAPI Game {
#if defined(PLATFORM_UWP)
        friend ref class UWP::GameApp;
#endif
        /**
         * The game window.
         */
        Window *m_gameWindow = nullptr;

        /**
         * The config to create the game window with.
         */
        WindowConfig m_gameWindowCreationConfig;

        /**
         * The game renderer.
         */
        graphics::Renderer *m_renderer = nullptr;

        /**
         * The currently loaded scene
         */
        Scene *m_currentScene = nullptr;

        /**
         * The render target used for enforcing resolution
         */
        graphics::RenderTarget *m_renderTarget = nullptr;

        /**
         * Is the game loop running
         */
        bool m_running = false;

        /**
         * The game update timestep.
         */
        std::chrono::milliseconds m_timestep;

        /**
         * The time that the game was started.
         */
        std::chrono::time_point<std::chrono::high_resolution_clock> m_started;

        /**
         * Lag. Used to run game updates.
         */
        std::chrono::nanoseconds m_lag;

        /**
         * The update thread.
         */
        std::thread m_updateThread;

        /**
         * Whether or not the game has been suspended for being invisible.
         */
        bool m_suspended = false;

        /**
         * The resource manager.
         */
        filesystem::ResourceManager *m_resourceManager = nullptr;

        /**
         * The virtual viewport.
         */
        graphics::Viewport m_virtualViewport;

        /**
         * Initialize the game ready to process frames.
         */
        void _init();

        /**
         * Draw/handle window events.
         */
        void _runDraw();

        /**
         * Game update logic.
         */
        void _runUpdate();

        /**
         * The update thread.
         */
        void _updateThread();

        /**
         * Cleanup the game.
         */
        void _cleanup();
    public:
        /**
         * Background clear color.
         */
        graphics::Color ClearColor = graphics::Color::Black;

        /**
         * Game config.
         */
        GameConfig Config;

        /**
         * On resume event.
         * 
         * @note This event should be used to load game data after suspension (i.e. being maximized).
         */
        Event<> OnResume;

        /**
         * On suspension event.
         * 
         * @note This event should be used to save game data. It may mean either the game is closing *or* the game is suspending (For example when the game is minimized).
         */
        Event<> OnSuspend;

        /**
         * On update event
         */
        Event<graphics::Renderer *> OnDraw;

        /**
         * On update event
         */
        Event<> OnUpdate;

        /**
         * Filter mode for scaling render target
         */
        graphics::TextureFilterMode RenderTargetFilterMode = graphics::TextureFilterMode::Trilinear;

        /**
         * Create a new Game
         *
         * @param config The game configuration.
         */
        Game(WindowConfig windowConfig, const GameConfig &config);

        virtual ~Game() = default;

        /**
         * Initialize the game, allocating any resources needed.
         */
        virtual void initialize();

        /**
         * Cleanup the game, deleting any resources created.
         */
        virtual void cleanup();

        /**
         * Get the game's window.
         *
         * @return The game window.
         */
        Window *getGameWindow() const;

        /**
         * Get this game's mouse handler.
         */
        input::Mouse *getMouse() const;

        /**
         * Get this game's keyboard handler.
         */
        input::Keyboard *getKeyboard() const;

        /**
         * Get the graphics device.
         *
         * @return The graphics device.
         */
        graphics::GraphicsDevice *getGraphicsDevice();

        /**
         * Get the default OS window size
         *
         * @return The default window dimensions.
         */
        Vector2 getDefaultWindowDimensions() const;

        /**
         * Get game dimensions
         *
         * @return The game's virtual dimensions.
         */
        Vector2 getDimensions() const;

        /**
         * Get the size of the root viewport.
         *
         * This will either be the virtual viewport or the window viewport.
         */
        Rectangle getGameViewport();

        /**
         * Get the target FPS.
         *
         * @return The game target FPS.
         */
        int getTargetFPS() const;

        /**
         * Start the game loop.
         */
        void run();

        /**
         * Is the game running
         *
         * @return Whether or not the game is running.
         */
        bool isRunning();

        /**
         * Quit the game
         */
        void quit();

        /**
         * Set the target FPS.
         *
         * @param FPS The new target FPS.
         */
        void setFPS(int FPS);

        /**
         * Set the intended game size.
         * Will resize the framebuffer next frame.
         *
         * @param size The new virtual resolution for the game.
         */
        void setIntendedSize(Vector2 size);

        /**
         * Set whether or not the game is running
         *
         * @param running Whether or not the game should be running.
         */
        void setIsRunning(bool running);

        /**
         * Set the current scene
         *
         * @param scene The scene to use now.
         */
        void setScene(Scene *scene);

        /**
         * Get the game resource manager.
         *
         * @return The game resource manager.
         */
        filesystem::ResourceManager *getResourceManager() const;
    };
}

#endif //GAME_HPP
