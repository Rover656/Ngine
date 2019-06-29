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

// Angle maths
#define RadToDeg(rad) rad * 180/3.14
#define DegToRad(deg) deg * 3.14/180

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
#include <stdexcept>
#include <string>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#if defined(INCLUDE_RAYLIB)
#include <raylib.h>    // Required for: raylib in its entirety
#endif

// Remove stuff from Windows.h that break Ngine and raylib
#ifdef _WIN32
#define NOGDI
#define NODRAWTEXT
#define NOUSER
#define NOCRYPT
#endif

//----------------------------------------------------------------------------------
// Helper Functions
//----------------------------------------------------------------------------------

void NEAPI ConsoleMessage(std::string message, std::string severity = "NOTICE", std::string module = "NGINE");

//----------------------------------------------------------------------------------
// Enums
//----------------------------------------------------------------------------------

namespace NerdThings::Ngine {
    /*
     * Game config.
     * Backward compatible with raylib as the same enums have the same values
     */
    enum EGameConfig {
        /*
         * None
         */
        NONE = 1,

        /*
         * Start in fullscreen?
         */
        FULLSCREEN = 2,

        /*
         * Have a resizeable window
         * Should usually be used with MAINTAIN_DIMENSIONS
         */
        RESIZEABLE_WINDOW = 4,

        /*
         * Have a frameless window
         */
        FRAMELESS_WINDOW = 8,

        /*
         * Have a transparent window
         */
        TRANSPARENT_WINDOW = 16,

        /*
         * 4x Multi-sample Anti-aliasing
         */
        MSAA_4X = 32,

        /*
         * Vertical sync
         */
        VSYNC = 64,

        /*
         * Start the game with a hidden window
         */
        HIDDEN_WINDOW = 128,

        /*
         * Continue running game when minimized
         */
        ALWAYS_RUN_MINIMIZED = 256,

        /*
         * Whether or not to maintain the dimensions provided to the game constructor
         */
        MAINTAIN_DIMENSIONS = 512
    };

    /*
     * Gamepad axis
     */
    enum EGamepadAxis {
        /*
         * Error checking for unknown axis used
         */
        GAMEPAD_AXIS_UNKNOWN = 0,

        /*
         * Left stick X
         */
        GAMEPAD_AXIS_LEFT_X,

        /*
         * Left stick Y
         */
        GAMEPAD_AXIS_LEFT_Y,

        /*
         * Right stick X
         */
        GAMEPAD_AXIS_RIGHT_X,

        /*
         * Right stick Y
         */
        GAMEPAD_AXIS_RIGHT_Y,

        /*
         * Left trigger
         */
        GAMEPAD_AXIS_LEFT_TRIGGER,

        /*
         * Right trigger
         */
        GAMEPAD_AXIS_RIGHT_TRIGGER
    };

    /*
     * Gamepad buttons
     */
    enum EGamepadButton {
        /*
         * Error checking for unknown buttons
         */
        GAMEPAD_BUTTON_UNKNOWN = 0,

        /*
         * Left collection of buttons up
         */
        GAMEPAD_BUTTON_LEFT_FACE_UP,

        /*
         * Left collection of buttons right
         */
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,

        /*
         * Left collection of buttons down
         */
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,

        /*
         * Left collection of buttons left
         */
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,

        /*
         * Right collection of buttons up
         */
        GAMEPAD_BUTTON_RIGHT_FACE_UP,

        /*
         * Right collection of buttons right
         */
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,

        /*
         * Right collection of buttons down
         */
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,

        /*
         * Right collection of buttons left
         */
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,

        /*
         * Left trigger 1
         */
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,

        /*
         * Left trigger 2
         */
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,

        /*
         * Right trigger 1
         */
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,

        /*
         * Right trigger 2
         */
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,

        /*
         * Middle left button
         */
        GAMEPAD_BUTTON_MIDDLE_LEFT,

        /*
         * Middle button
         */
        GAMEPAD_BUTTON_MIDDLE,

        /*
         * Middle right button
         */
        GAMEPAD_BUTTON_MIDDLE_RIGHT,

        /*
         * Left thumb clicked in
         */
        GAMEPAD_BUTTON_LEFT_THUMB,

        /*
         * Right thumb clicked in
         */
        GAMEPAD_BUTTON_RIGHT_THUMB
    };

    /*
     * Gamepad numbers
     */
    enum EGamepadNumber {
        /*
         * Gamepad 1
         */
        GAMEPAD_1 = 0,

        /*
         * Gamepad 2
         */
        GAMEPAD_2 = 1,

        /*
         * Gamepad 3
         */
        GAMEPAD_3 = 2,

        /*
         * Gamepad 4
         */
        GAMEPAD_4 = 3
    };

    /*
     * Mouse button
     */
    enum EMouseButton {
        MOUSE_BUTTON_LEFT = 0,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE
    };

    /*
     * A keyboard key
     */
     // Yes, this is taken from raylib for compatibility purposes
    enum EKey {
        // For special purposes
        KEY_NONE = 0,

        // Alphanumeric keys
        KEY_APOSTROPHE = 39,
        KEY_COMMA = 44,
        KEY_MINUS = 45,
        KEY_PERIOD = 46,
        KEY_SLASH = 47,
        KEY_ZERO = 48,
        KEY_ONE = 49,
        KEY_TWO = 50,
        KEY_THREE = 51,
        KEY_FOUR = 52,
        KEY_FIVE = 53,
        KEY_SIX = 54,
        KEY_SEVEN = 55,
        KEY_EIGHT = 56,
        KEY_NINE = 57,
        KEY_SEMICOLON = 59,
        KEY_EQUAL = 61,
        KEY_A = 65,
        KEY_B = 66,
        KEY_C = 67,
        KEY_D = 68,
        KEY_E = 69,
        KEY_F = 70,
        KEY_G = 71,
        KEY_H = 72,
        KEY_I = 73,
        KEY_J = 74,
        KEY_K = 75,
        KEY_L = 76,
        KEY_M = 77,
        KEY_N = 78,
        KEY_O = 79,
        KEY_P = 80,
        KEY_Q = 81,
        KEY_R = 82,
        KEY_S = 83,
        KEY_T = 84,
        KEY_U = 85,
        KEY_V = 86,
        KEY_W = 87,
        KEY_X = 88,
        KEY_Y = 89,
        KEY_Z = 90,

        // Function keys
        KEY_SPACE = 32,
        KEY_ESCAPE = 256,
        KEY_ENTER = 257,
        KEY_TAB = 258,
        KEY_BACKSPACE = 259,
        KEY_INSERT = 260,
        KEY_DELETE = 261,
        KEY_RIGHT = 262,
        KEY_LEFT = 263,
        KEY_DOWN = 264,
        KEY_UP = 265,
        KEY_PAGE_UP = 266,
        KEY_PAGE_DOWN = 267,
        KEY_HOME = 268,
        KEY_END = 269,
        KEY_CAPS_LOCK = 280,
        KEY_SCROLL_LOCK = 281,
        KEY_NUM_LOCK = 282,
        KEY_PRINT_SCREEN = 283,
        KEY_PAUSE = 284,
        KEY_F1 = 290,
        KEY_F2 = 291,
        KEY_F3 = 292,
        KEY_F4 = 293,
        KEY_F5 = 294,
        KEY_F6 = 295,
        KEY_F7 = 296,
        KEY_F8 = 297,
        KEY_F9 = 298,
        KEY_F10 = 299,
        KEY_F11 = 300,
        KEY_F12 = 301,
        KEY_LEFT_SHIFT = 340,
        KEY_LEFT_CONTROL = 341,
        KEY_LEFT_ALT = 342,
        KEY_LEFT_SUPER = 343,
        KEY_RIGHT_SHIFT = 344,
        KEY_RIGHT_CONTROL = 345,
        KEY_RIGHT_ALT = 346,
        KEY_RIGHT_SUPER = 347,
        KEY_KB_MENU = 348,
        KEY_LEFT_BRACKET = 91,
        KEY_BACKSLASH = 92,
        KEY_RIGHT_BRACKET = 93,
        KEY_GRAVE = 96,

        // Keypad keys
        KEY_KP_0 = 320,
        KEY_KP_1 = 321,
        KEY_KP_2 = 322,
        KEY_KP_3 = 323,
        KEY_KP_4 = 324,
        KEY_KP_5 = 325,
        KEY_KP_6 = 326,
        KEY_KP_7 = 327,
        KEY_KP_8 = 328,
        KEY_KP_9 = 329,
        KEY_KP_DECIMAL = 330,
        KEY_KP_DIVIDE = 331,
        KEY_KP_MULTIPLY = 332,
        KEY_KP_SUBTRACT = 333,
        KEY_KP_ADD = 334,
        KEY_KP_ENTER = 335,
        KEY_KP_EQUAL = 336
    };

    /*
     * Texture filter mode
     */
    enum ETextureFilterMode {
        /*
         * No filter, just approximation
         */
        FILTER_POINT = 0,

        /*
         * Linear filtering
         */
        FILTER_BILINEAR,

        /*
         * Trilinear filtering (with mipmaps)
         */
        FILTER_TRILINEAR,

        /*
         * Anisotropic filtering 4x
         */
        FILTER_ANISOTROPIC_4X,

        /*
         * Anisotropic filtering 8x
         */
        FILTER_ANISOTROPIC_8X,

        /*
         * Anisotropic filtering 16x
         */
        FILTER_ANISOTROPIC_16X
    };

    /*
     * Texture wrap mode
     */
    enum ETextureWrapMode {
        /*
         * Repeats texture
         */
        WRAP_REPEAT = 0,

        /*
         * Clamps texture to edge pixel
         */
        WRAP_CLAMP,

        /*
         * Mirrors and repeats the texture
         */
        WRAP_MIRROR_REPEAT,

        /*
         * Mirrors and clamps to the border
         */
        WRAP_MIRROR_CLAMP
    };

    /*
     * UI Panel horizontal alignment
     */
    enum EUIPanelHorizontalAlignment {
        /*
         * Align elements with the left side of the panel
         */
        ALIGN_LEFT = 0,

        /*
         * Align elements with the middle of the panel
         */
        ALIGN_CENTER,

        /*
         * Align elements with the right side of the panel
         */
        ALIGN_RIGHT
    };

    /*
     * UI Panel vertical alignment
     */
    enum EUIPanelVerticalAlignment {
        /*
         * Align elements with top of panel
         */
        ALIGN_TOP = 0,

        /*
         * Align elements with middle of panel
         */
        ALIGN_MIDDLE,

        /*
         * Align elements with bottom of panel
         */
        ALIGN_BOTTOM
    };
}

#endif // NGINE_H
