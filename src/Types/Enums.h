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

#ifndef ENUMS_H
#define ENUMS_H

namespace NerdThings::Ngine {
    /*
     * Game config.
     * Backward compatible with raylib as the same enums have the same values
     */
    enum EGameConfig {
        /*
         * None
         */
        NONE = 1,

        /*
         * Start in fullscreen?
         */
        FULLSCREEN = 2,

        /*
         * Have a resizeable window
         * Should usually be used with MAINTAIN_DIMENSIONS
         */
        RESIZEABLE_WINDOW = 4,

        /*
         * Have a frameless window
         */
        FRAMELESS_WINDOW = 8,

        /*
         * Have a transparent window
         */
        TRANSPARENT_WINDOW = 16,

        /*
         * 4x Multi-sample Anti-aliasing
         */
        MSAA_4X = 32,

        /*
         * Vertical sync
         */
        VSYNC = 64,

        /*
         * Start the game with a hidden window
         */
        HIDDEN_WINDOW = 128,

        /*
         * Whether or not to maintain the dimensions provided to the game constructor
         */
        MAINTAIN_DIMENSIONS = 256
    };

    /*
     * Mouse button
     */
    enum EMouseButton {
        LEFT = 0,
        RIGHT,
        MIDDLE
    };
}

#endif //ENUMS_H
