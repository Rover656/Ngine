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
// Basic Definitions
//----------------------------------------------------------------------------------

#if defined(NGINE_BUILD)
#define INCLUDE_BOX2D
#endif

#if defined(_WIN32) && defined(NGINE_EXPORTS)
#define NEAPI __declspec(dllexport)         // Windows DLL Export
#elif defined(_WIN32) && defined(NGINE_SHARED)
#define NEAPI __declspec(dllimport)         // Windows DLL Import
#else
#define NEAPI
#endif

// Root Ngine namespace
#define NGINE_NS NerdThings::Ngine

// Angle maths
#define PI 3.1415

#define RadToDeg(rad) rad * 180/PI
#define DegToRad(deg) deg * PI/180

//----------------------------------------------------------------------------------
// Project Info
//----------------------------------------------------------------------------------

#define NGINE_VERSION "0.1a"

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

#if defined(PLATFORM_UWP)
// Fix UWP??
#define NOMINMAX
#include <Windows.h>
#endif

#if defined(INCLUDE_BOX2D)
#include <Box2D/Box2D.h>

// For simpler testing while we have a janky box2d impl
// Will be removed and used properly after the game jam, this is a quick fix because it is important
bool NEAPI b2TestOverlap(const b2Shape* shapeA, const b2Shape* shapeB);
#endif

// Remove stuff from Windows.h that break Ngine and raylib
#ifdef _WIN32
#define NOGDI
#define NODRAWTEXT
#define NOBITMAP
#endif

//----------------------------------------------------------------------------------
// Helper Functions
//----------------------------------------------------------------------------------

void NEAPI ConsoleMessage(std::string message, std::string severity = "NOTICE", std::string module = "NGINE");

#endif // NGINE_H
