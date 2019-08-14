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

#include "ngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef INCLUDE_BOX2D
bool b2TestOverlap(const b2Shape* shapeA, const b2Shape* shapeB) {
    b2Transform nope;
    nope.SetIdentity();
    return b2TestOverlap(shapeA, 0, shapeB, 0, nope, nope);
}
#endif

// ConsoleMessage
void ConsoleMessage(std::string message, std::string severity, std::string module) {
    {
        std::cout << "[" + module + "] " + severity + ": " + message << std::endl;
    }
}
