/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef CONFIG_HPP
#define CONFIG_HPP

//----------------------------------------------------------------------------------
// Common includes
//----------------------------------------------------------------------------------

// STL Stuff
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

// Include Windows
#if defined(_WIN32)
#define NOBITMAP
#define NODRAWTEXT
#define NOGDI
#define NOMINMAX
#include <Windows.h>
#endif

//----------------------------------------------------------------------------------
// Basic Definitions
//----------------------------------------------------------------------------------

// DLL imports
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
// Platform Identification
//----------------------------------------------------------------------------------

#if defined(_WIN32)
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
        #define PLATFORM_UWP
    #else
        #define PLATFORM_DESKTOP
    #endif
#elif defined(__APPLE__)
    #include "TargetConditionals.h"
    #if defined(TARGET_OS_MAC)
        #define PLATFORM_DESKTOP
    #else
        #error "Unsupported platform"
    #endif
#elif __linux__
    #define PLATFORM_DESKTOP
#else
    #error "Unsupported platform"
#endif

//#define USE_EXPERIMENTAL_RENDERER // TEMP: UWP

#endif //CONFIG_HPP