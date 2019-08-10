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
#include "Resources.h"
#include "Vector2.h"
#include "EventHandler.h"
#include "Scene.h"

namespace NerdThings::Ngine {
    /*
     * The main container of the game
     */
    class NEAPI Game {
        // Private Fields

        /*
         * The game config (stored)
         */
        int _Config;

        /*
         * The currently loaded scene
         */
        Scene *_CurrentScene = nullptr;

        /*
         * The target draw FPS
         */
        int _DrawFPS = 0;

        /*
         * The intended game height
         */
        int _IntendedHeight = 0;

        /*
         * The intended game width
         */
        int _IntendedWidth = 0;

        /*
         * The render target used for enforcing resolution
         */
        Graphics::TRenderTarget _RenderTarget;

        /*
         * Is the game loop running
         */
        bool _Running = false;

        /*
         * The target update FPS
         */
        int _UpdateFPS = 0;

        /*
         * The default window height
         */
        int _WindowHeight = 0;

        /*
         * The default window width
         */
        int _WindowWidth = 0;

    public:
        // Public Fields

        /*
         * Background clear color
         */
        Graphics::TColor ClearColor = Graphics::TColor::Black;

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
        ETextureFilterMode RenderTargetFilterMode = FILTER_BILINEAR;

        // Public Constructor(s)

        /*
         * Create a new Game
         */
        Game(int width_, int height_, int FPS_, const std::string &title_, int config_ = NONE);

        /*
         * Create a new Game (Extra FPS options)
         */
        Game(int width_, int height_, int drawFPS_, int updateFPS_, const std::string &title_, int config_ = NONE);

        /*
         * Create a new Game (Advanced)
         */
        Game(int windowWidth_, int windowHeight_, int targetWidth_, int targetHeight_, int drawFPS_, int updateFPS_,
             const std::string &title_, int config_ = NONE);

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
         * Get the game config
         */
        int GetConfig();

        /*
         * Get the target FPS.
         * If using advanced game will return update FPS
         */
        [[nodiscard]] int GetFPS() const;

        /*
         * Get the default OS window size
         */
        TVector2 GetDefaultWindowDimensions() const;

        /*
         * Get game dimensions
         */
        TVector2 GetDimensions() const;

        /*
         * Get the target draw FPS.
         */
        [[nodiscard]] int GetDrawFPS() const;

        /*
         * Get the target update FPS.
         */
        [[nodiscard]] int GetUpdateFPS() const;

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
        void SetIntendedSize(TVector2 size_);

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
