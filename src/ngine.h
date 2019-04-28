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

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include <chrono>                           // Required for std::chrono timings
#include <string>                           // Required for std::string

//----------------------------------------------------------------------------------
// Basic Definitions
//----------------------------------------------------------------------------------

#if defined(_WIN32) && defined(BUILD_SHARED)
#define NEAPI __declspec(dllexport)         // Windows DLL Export
#elif defined(_WIN32) && defined(USE_SHARED)
#define NEAPI __declspec(dllimport)         // Windows DLL Import
#else
#define NEAPI
#endif

#endif // NGINE_H
