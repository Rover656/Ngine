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

#if defined(NGINE_EXPORTS)
#define INCLUDE_RAYLIB
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

//----------------------------------------------------------------------------------
// Libraries
//----------------------------------------------------------------------------------

// C++ Includes
#include <algorithm>                            // Required for: std::clamp and alike methods
#include <chrono>                               // Required for: std::chrono timings
#include <string>                               // Required for: std::string
#include <map>                                  // Required for: std::map
#include <thread>                               // Required for: std::this_thread sleep
#include <vector>                               // Required for: std::vector

#if defined(INCLUDE_RAYLIB)
#include "3rd-party/raylib/src/raylib.h"    // Required for: raylib in its entirety
#endif

// Type includes
#if !defined(TYPE_DECL_HEADER)
#include "Types/BoundingBox2D.h"
#include "Types/Camera2D.h"
#include "Types/Circle.h"
#include "Types/CollisionShape.h"
#include "Types/Color.h"
#include "Types/Enums.h"
#include "Types/EventArgs.h"
#include "Types/Font.h"
#include "Types/Matrix.h"
#include "Types/Rectangle.h"
#include "Types/RenderTarget.h"
#include "Types/Texture2D.h"
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#endif

// Remove stuff from Windows.h that break Ngine and raylib
#ifdef _WIN32
#define NOGDI
#define NODRAWTEXT
#define NOUSER
#endif

#endif // NGINE_H
