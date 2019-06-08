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

#include "../ngine.h"

#include "../Graphics/Color.h"
#include "../Graphics/Drawing.h"
#include "../Graphics/RenderTarget.h"
#include "../Math/Vector2.h"
#include "EventHandler.h"
#include "Scene.h"

namespace NerdThings::Ngine::Core {
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
         * The target update FPS
         */
        int _UpdateFPS = 0;

    public:
        // Public Fields

        /*
         * Background clear color
         */
        Graphics::TColor ClearColor = Graphics::TColor::Black;

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

        virtual ~Game() = default;

        // Public Methods

        /*
         * Draw a frame.
         */
        void Draw();

        /*
         * Get the target FPS.
         * If using advanced game will return update FPS
         */
        [[nodiscard]] int GetFPS() const;

        /*
         * Get game dimensions
         */
        Math::TVector2 GetDimensions() const;

        /*
         * Get the target draw FPS.
         */
        [[nodiscard]] int GetDrawFPS() const;

        /*
         * Get the target update FPS.
         */
        [[nodiscard]] int GetUpdateFPS() const;

        /*
         * Start the game loop
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
        void SetIntendedSize(Math::TVector2 size_);

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

    protected:
        // Protected Methods

        /*
         * Clear the game background
         */
        void Clear() const;
    };
}

#endif //GAME_H
