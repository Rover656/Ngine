/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "Ngine.h"

#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderTarget.h"
#include "Filesystem/Resources.h"
#include "EventHandler.h"
#include "Scene.h"
#include "Window.h"
#include "Vector2.h"

namespace NerdThings::Ngine {
    /**
     * Game configuration structure.
     * This details exactly how the game should be configured.
     */
    struct GameConfig {
        // Public Fields

        /**
         * Number of times to draw every second
         */
        int FPS = 60;

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
        // Private Fields

        /**
         * The currently loaded scene
         */
        Scene *_CurrentScene = nullptr;

        /**
         * The render target used for enforcing resolution
         */
        std::shared_ptr<Graphics::RenderTarget> _RenderTarget = nullptr;

        /**
         * Is the game loop running
         */
        bool _Running = false;

        // Private Methods

        /**
         * Render a frame
         */
        void __DoDraw();

        /**
         * Run a game update
         */
        void __DoUpdate();

    public:
        // Public Fields

        /**
         * Background clear color
         */
        Graphics::Color ClearColor = Graphics::Color::Black;

        /**
         * Game config
         */
        GameConfig Config;

        /**
         * On update event
         */
        Event<> OnDraw;

        /**
         * On init event
         */
        Event<> OnInit;

        /**
         * On update event
         */
        Event<> OnUpdate;

        /**
         * Filter mode for scaling render target
         */
        Graphics::TextureFilterMode RenderTargetFilterMode = Graphics::FILTER_BILINEAR;

        // Public Constructor(s)

        /**
         * Create a new Game
         *
         * @param config_ The game configuration.
         */
        explicit Game(const GameConfig &config_);

        // Destructor

        virtual ~Game();

        // Public Methods

        /**
         * Get the default OS window size
         *
         * @returns The default window dimensions.
         */
        Vector2 GetDefaultWindowDimensions() const;

        /**
         * Get game dimensions
         *
         * @returns The game's virtual dimensions.
         */
        Vector2 GetDimensions() const;

        /**
         * Get the target FPS.
         *
         * @returns The game target FPS.
         */
        int GetTargetFPS() const;

        /**
         * Is the game running
         *
         * @returns Whether or not the game is running.
         */
        bool IsRunning();

        /**
         * Quit the game
         */
        void Quit();

        /**
         * Start the game loop.
         */
        void Run();

        /**
         * Set the target FPS.
         *
         * @param FPS_ The new target FPS.
         */
        void SetFPS(int FPS_);

        /**
         * Set the intended game size.
         * Will resize the framebuffer next frame.
         *
         * @param size_ The new virtual resolution for the game.
         */
        void SetIntendedSize(Vector2 size_);

        /**
         * Set whether or not the game is running
         *
         * @param running_ Whether or not the game should be running.
         */
        void SetIsRunning(bool running_);

        /**
         * Set the current scene
         *
         * @param scene_ The scene to use now.
         */
        void SetScene(Scene *scene_);
    };
}

#endif //GAME_H
