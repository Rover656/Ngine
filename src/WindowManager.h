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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "ngine.h"

namespace NerdThings::Ngine {
    /*
     * Window management
     */
    class NEAPI WindowManager {
    public:
        // Public Methods

        /*
         * Applies window-oriented config.
         * It is recommended to use Game::SetConfig instead.
         */
        static void ApplyConfig(int config_);

        /*
         * Close the current window
         */
        static void Close();

        /*
         * Get the window height
         */
        static int GetWindowHeight();

        /*
         * Get the window width
         */
        static int GetWindowWidth();

        /*
         * Hide the cursor
         */
        static void HideCursor();

        /*
         * Hide window (Desktop)
         */
        static void HideWindow();

        /*
         * Init the window
         */
        static void Init(int width_, int height_, const std::string &title_);

        /*
         * Is the cursor visible
         */
        static bool IsCursorVisible();

        /*
         * Is the window minimized (Desktop)
         */
        static bool IsMinimized();

        /*
         * Is the window hidden (Desktop)
         */
        static bool IsHidden();

        // TODO: SetWindowIcon?

        /*
         * Set window minimum size (Desktop)
         */
        static void SetMinSize(int width_, int height_);

        /*
         * Set which monitor the window is on (Desktop)
         */
        static void SetMonitor(int monitor_);

        /*
         * Set window position (Desktop)
         */
        static void SetPosition(int x_, int y_);

        /*
         * Set window size (Desktop)
         */
        static void SetSize(int width_, int height_);

        /*
         * Set the target fps
         */
        static void SetTargetFPS(int fps_);

        /*
         * Set window title (Desktop)
         */
        static void SetTitle(const std::string &title_);

        /*
         * Should the window close
         */
        static bool ShouldClose();

        /*
         * Make the cursor visible
         */
        static void ShowCursor();

        /*
         * Toggle fullscreen mode (Desktop)
         */
        static void ToggleFullscreen();

        /*
         * Unhide window (Desktop)
         */
        static void UnHideWindow();
    };
}

#endif //WINDOWMANAGER_H
