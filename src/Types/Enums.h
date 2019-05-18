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

/*
 * Game config
 */
enum EGameConfig {
    NONE = 0,
    FULLSCREEN = 2,
    RESIZEABLE_WINDOW = 4,
    FRAMELESS_WINDOW = 8,
    TRANSPARENT_WINDOW = 16,
    //HIDDEN_WINDOW = 128, // Removed for now because we can't unhide a window. Idk if we need this.
    MSAA_4X = 32,
    VSYNC = 64
};

/*
 * Mouse button
 */
enum EMouseButton {
    LEFT = 0,
    RIGHT,
    MIDDLE
};

#endif //ENUMS_H
