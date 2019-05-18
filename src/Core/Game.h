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

#include "../Graphics/Drawing.h"
#include "EventHandler.h"

namespace NerdThings::Ngine::Core {
    /*
     * The main container of the game
     */
    class NEAPI Game {
        // Private Fields

        /*
         * The currently loaded scene
         */
        BaseScene *_CurrentScene = nullptr;

        /*
         * The target draw FPS
         */
        int _DrawFPS = 0;

        /*
         * The target update FPS
         */
        int _UpdateFPS = 0;

    public:
        // Public Fields

        /*
         * Background clear color
         */
        TColor ClearColor = TColor::Black;

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

        // Public Constructor(s)

        /*
         * Create a new Game
         */
        Game(int width_, int height_, int FPS_, const std::string &title_, int config_ = None);

        /*
         * Create a new Game (Advanced)
         */
        Game(int width_, int height_, int drawFPS_, int updateFPS_, const std::string &title_, int config_ = None);

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
         * Set the current scene
         */
        void SetScene(BaseScene *scene_);

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
