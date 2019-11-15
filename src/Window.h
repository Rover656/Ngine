/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "Ngine.h"

#if defined(PLATFORM_UWP)
namespace NerdThings::Ngine::UWP {
    ref class GameApp;
}

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;
#endif

#include <fstream>
#include <iostream>

#include "Graphics/Image.h"
#include "EventHandler.h"

namespace NerdThings::Ngine {
    /**
     * Window configuration.
     * Details exactly how the window will be created.
     */
    struct WindowConfig {
        /**
         * Display a native console for writing to.
         *
         * @note Currently windows only.
         */
        bool NativeDebugConsole = false;

        /**
         * Fullscreen window.
         */
        bool Fullscreen = false;

        /**
         * Window icon.
         */
        Graphics::Image *Icon = nullptr;

        /**
         * Initial window height.
         */
        int InitialHeight = 0;

        /**
         * Initial window width.
         */
        int InitialWidth = 0;

        /**
         * Enable MSAA 4X.
         *
         * @warning Cannot be changed after creation.
         * @note Does not currently work on UWP.
         */
        bool MSAA_4X = false;

        /**
         * Whether or not the window is resizable.
         */
        bool Resizable = false;

        /**
         * The window title.
         *
         * @note Only works on Desktop platforms.
         */
        std::string Title = "Game Window";

        /**
         * Whether or not to use V-Sync.
         */
        bool VSync = false;
    };

    /**
     * Ngine window management wrapper.
     */
    class NEAPI Window {
        // Private Fields

        /**
         * Current window height
         */
        static int _CurrentHeight;

        /**
         * Current window width
         */
        static int _CurrentWidth;

        /**
         * Whether or not the window is initialized
         */
        static bool _Initialized;

        /**
         * Is the window currently fullscreen
         */
        static bool _IsFullscreen;

        /**
         * X Position of window before fullscreen.
         */
        static int _PreFullscreenPosX;

        /**
         * Y Position of window before fullscreen.
         */
        static int _PreFullscreenPosY;

        /**
         * Width of window before fullscreen.
         */
        static int _PreFullscreenSizeWidth;

        /**
         * Height of window before fullscreen.
         */
        static int _PreFullscreenSizeHeight;

#if defined(PLATFORM_DESKTOP) && defined(_WIN32)
        /**
         * Whether or not a native console is allocated
         */
        static bool _ConsoleAllocated;

        /**
         * Console input stream
         */
        static std::fstream _ConsoleIn;

        /**
         * Console output stream
         */
        static std::fstream _ConsoleOut;

        /**
         * Console error stream
         */
        static std::fstream _ConsoleErr;

        /**
         * Pointer to original input buffer
         */
        static std::streambuf *_OldCinBuffer;

        /**
         * Pointer to original output buffer
         */
        static std::streambuf *_OldCoutBuffer;

        /**
         * Pointer to original error buffer
         */
        static std::streambuf *_OldCerrBuffer;
#endif
    public:
        // Public Fields

        /**
         * The current window config.
         * When modified, you must call ApplyConfig().
         */
        static WindowConfig Config;

#if defined(PLATFORM_DESKTOP)
        /**
         * GLFW Window pointer.
         */
        static void *WindowPtr;
#elif defined(PLATFORM_UWP)
        /**
         * EGL Context
         */
        static EGLContext Context;

        /**
         * EGL Display
         */
        static EGLDisplay Display;

        /**
         * EGL Surface
         */
        static EGLSurface Surface;
#endif

        // Public Methods

        /**
         * Apply changes in config
         */
        static void ApplyConfig();

        /**
         * Close window and clean API.
         *
         * @warning This will delete all data on the GPU. Rendering will no longer be possible.
         */
        static void Close();

        /**
         * Get window height.
         *
         * @returns Current window height.
         */
        static int GetHeight();

        /**
         * Get window width.
         *
         * @returns Current window width.
         */
        static int GetWidth();

        /**
         * Create a new window.
         */
        static void Init();

        /**
         * Determine whether or not the window is focussed on.
         *
         * @returns Whether or not the window has focus.
         * @note This only works on Desktop and UWP platforms.
         */
        static bool IsFocussed();

        /**
         * Poll window events
         */
        static void PollEvents();

        /**
         * Resize the window
         *
         * @param width_ New window width.
         * @param height_ New window height.
         */
        static void Resize(int width_, int height_);

        /**
         * Set window config.
         * Must be set before Initialization.
         *
         * @param config_ The new window configuration.
         */
        static void SetConfig(const WindowConfig &config_);

        /**
         * Set fullscreen.
         *
         * @param fullscreen_ Whether the window should be fullscreen.
         */
        static void SetFullscreen(bool fullscreen_);

        /**
         * Set whether or not the window can be resized.
         *
         * @param resizable_ Whether the window can be resized.
         * @note Only works on Desktop platforms.
         */
        static void SetResizable(bool resizable_);

        /**
         * Set window title.
         *
         * @param title_ New window title.
         * @note Only works on Desktop platforms.
         */
        static void SetTitle(const std::string& title_);

        /**
         * Enable or disable vsync.
         *
         * @param vsync_ Whether vsync is on or off.
         */
        static void SetVSync(bool vsync_);

        /**
         * Determine Whether or not the program should close.
         *
         * @returns Whether or not the program should close.
         */
        static bool ShouldClose();

        /**
         * Swap the window buffers
         */
        static void SwapBuffers();

        /**
         * Whether or not the window is visible.
         *
         * @returns If the window is visible or not.
         */
        static bool Visible();
    };
}

#endif //WINDOW_H
