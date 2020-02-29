/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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

// Angle maths
#define PI 3.1415

#define RadToDeg(rad) (rad) * 180/PI
#define DegToRad(deg) (deg) * PI/180

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

#endif // CONFIG_HPP