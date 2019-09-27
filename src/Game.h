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

#include "ngine.h"

#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderTarget.h"
#include "Filesystem/Resources.h"
#include "EventHandler.h"
#include "Scene.h"
#include "Window.h"
#include "Vector2.h"

namespace NerdThings::Ngine {
    struct GameConfig {
        // Public Fields

        /*
         * Number of times to draw every second
         */
        int DrawFPS = 60;

        /*
         * Target game height
         */
        int TargetHeight;

        /*
         * Maintain game virtual resolution
         */
        bool MaintainResolution = false;

        /*
         * Run the game while not visible.
         */
        bool RunWhileHidden = true;

        /*
         * Target game width
         */
        int TargetWidth = 0;

        /*
         * Number of times to update very second
         */
        int UpdateFPS = 60;

        // Public Methods

        /*
         * Set FPS (simple)
         */
        void SetFPS(int fps_) {
            DrawFPS = fps_;
            UpdateFPS = fps_;
        }
    };

    /*
     * The main container of the game
     */
    class NEAPI Game {
        // Private Fields

        /*
         * The currently loaded scene
         */
        Scene *_CurrentScene = nullptr;

        /*
         * The render target used for enforcing resolution
         */
        std::shared_ptr<Graphics::RenderTarget> _RenderTarget;

        /*
         * Is the game loop running
         */
        bool _Running = false;

    public:
        // Public Fields

        /*
         * Background clear color
         */
        Graphics::Color ClearColor = Graphics::Color::Black;

        /*
         * Game config
         */
        GameConfig Config;

        /*
         * The current game. If this isn't nullptr at start, it will error
         */
        static Game *CurrentGame;

        /*
         * On update event
         */
        EventHandler<EventArgs> OnDraw;

        /*
         * On run event
         */
        EventHandler<EventArgs> OnRun;

        /*
         * On update event
         */
        EventHandler<EventArgs> OnUpdate;

        /*
         * Filter mode for scaling render target
         */
        Graphics::TextureFilterMode RenderTargetFilterMode = Graphics::FILTER_BILINEAR;

        // Public Constructor(s)

        /*
         * Create a new Game
         */
        Game(const GameConfig &config_);

        // Destructor

        virtual ~Game();

        // Public Methods

        /*
         * Clear the game background
         */
        void Clear() const;

        /*
         * Draw a frame.
         */
        void Draw();

        /*
         * Get the default OS window size
         */
        Vector2 GetDefaultWindowDimensions() const;

        /*
         * Get game dimensions
         */
        Vector2 GetDimensions() const;

        /*
         * Get the target draw FPS.
         */
        int GetDrawFPS() const;

        /*
         * Get the target update FPS.
         */
        int GetUpdateFPS() const;

        /*
         * Is the game running
         */
        bool IsRunning();

        /*
         * Quit the game
         */
        void Quit();

        /*
         * Start the game loop.
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
         * Set the intended game size.
         * Resizes the framebuffer next frame.
         */
        void SetIntendedSize(Vector2 size_);

        /*
         * Set whether or not the game is running
         */
        void SetIsRunning(bool running_);

        /*
         * Set the current scene
         */
        void SetScene(Scene *scene_);

        /*
         * Set the target update FPS
         */
        void SetUpdateFPS(int FPS_);

        /*
         * Update logic
         */
        void Update();
    };
}

#endif //GAME_H
