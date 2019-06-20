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

// cute_c2 definitions
#define CUTE_C2_IMPLEMENTATION
#include <cute_c2.h>

// ConsoleMessage
void ConsoleMessage(std::string message, std::string severity, std::string module) {
    {
        std::cout << "[" + module + "] " + severity + ": " + message << std::endl;
    }
}
