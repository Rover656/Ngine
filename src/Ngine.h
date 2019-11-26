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

#ifndef NGINE_H
#define NGINE_H

#include "Config.h"

// TODO: Once this is clean, change all includes internally to Config and have all includes in here.

//----------------------------------------------------------------------------------
// Basic Definitions
//----------------------------------------------------------------------------------

// File includes TODO: Remove this need
#if defined(NGINE_BUILD)
#define INCLUDE_BOX2D
#endif

//----------------------------------------------------------------------------------
// Libraries
//----------------------------------------------------------------------------------

// C++ Includes
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#if defined(INCLUDE_BOX2D)
#include <Box2D/Box2D.h>

// For simpler testing while we have a janky box2d impl
// Will be removed and used properly after the game jam, this is a quick fix because it is important
bool NEAPI b2TestOverlap(const b2Shape* shapeA, const b2Shape* shapeB);
#endif

//----------------------------------------------------------------------------------
// Helper Functions
//----------------------------------------------------------------------------------

// TODO: Remove this in favour of `Logger`
void NEAPI ConsoleMessage(std::string message, std::string severity = "NOTICE", std::string module = "NGINE");

// TODO: Remove and add to cmake
// ENABLE EXPERIMENTAL RENDERER
#define USE_EXPERIMENTAL_RENDERER

#endif // NGINE_H
