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

#ifndef NGINECONFIG_H
#define NGINECONFIG_H

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

//----------------------------------------------------------------------------------
// Windows
//----------------------------------------------------------------------------------

#if defined(_WIN32)
// Remove stuff from Windows.h that breaks Ngine APIs
#define NOBITMAP
#define NODRAWTEXT
#define NOGDI
#define NOMINMAX

// Include Windows
#include <Windows.h>
#endif

// #define USE_EXPERIMENTAL_RENDERER

//----------------------------------------------------------------------------------
// Namespace Documentation
//----------------------------------------------------------------------------------

namespace NerdThings {
    /**
     * The main Ngine namespace.
     */
    namespace Ngine {
        /**
         * Ngine Audio module.
         */
        namespace Audio {}

        /**
         * This namespace consists of useful components.
         */
        namespace Components {}

        /**
         * Ngine Filesystem module.
         */
        namespace Filesystem {}

        /**
         * Ngine Graphics module.
         */
        namespace Graphics {
            /**
             * The experimental new renderer rewrite.
             */
            namespace Rewrite {}
        }

        /**
         * Ngine Input module.
         */
        namespace Input {}

        /**
         * Ngine Physics module.
         */
        namespace Physics {
            /**
             * Physics shapes.
             */
            namespace Shapes {}
        }

        /**
         * Ngine UI module.
         */
        namespace UI {}
    }
}

#endif //NGINECONFIG_H
