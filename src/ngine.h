/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*   
*   CONFIGURATION:
*   
*   - #define INCLUDE_RAYLIB
*       Include the raylib header
*   
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef NGINE_H
#define NGINE_H

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include <chrono>                   // Required for: std::chrono timings
#include <string>                   // Required for: std::string
#include <map>                      // Required for: std::map
#include <thread>                   // Required for: std::this_thread sleep
#include <vector>                   // Required for: std::vector

//----------------------------------------------------------------------------------
// Basic Definitions
//----------------------------------------------------------------------------------

#if defined(_WIN32) && defined(NGINE_EXPORTS)
#define NEAPI __declspec(dllexport)         // Windows DLL Export
#elif defined(_WIN32)
#define NEAPI __declspec(dllimport)         // Windows DLL Import
#else
#define NEAPI
#endif

// Use raylib DLL
#define USE_LIBTYPE_SHARED

#if defined(INCLUDE_RAYLIB) || defined(NGINE_EXPORTS) // NGINE_EXPORTS will mean we are building NGINE
#include "3rd-party/raylib/src/raylib.h"
#endif

#endif // NGINE_H
