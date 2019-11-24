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

#if defined(PLATFORM_DESKTOP)
typedef struct GLFWwindow GLFWwindow;
#elif defined(PLATFORM_UWP)
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
         * @note Only works on the Desktop platform.
         */
        std::string Title = "Game Window";

        /**
         * Whether or not to use V-Sync.
         * This will lock the framerate of your game to the monitors refresh rate.
         */
        bool VSync = false;
    };

    /**
     * Ngine window management wrapper.
     */
    class NEAPI Window {
        /**
         * The current window.
         */
         static Window *m_currentWindow;

#if defined(PLATFORM_DESKTOP)
        /**
         * GLFW Window.
         */
        GLFWwindow *m_GLFWWindow;
#elif defined(PLATFORM_UWP)
        /**
         * EGL Context
         */
        EGLContext m_context;

        /**
         * EGL Display
         */
        EGLDisplay m_display;

        /**
         * EGL Surface
         */
        EGLSurface m_surface;
#endif

        /**
         * Current window width.
         */
        int m_currentWidth = 0;

        /**
         * Current window height.
         */
        int m_currentHeight = 0;

        /**
         * Whether or not the window is initialized
         */
        bool m_initialized = false;

        /**
         * Is the window currently fullscreen
         */
        bool m_isFullscreen = false;

        /**
         * X Position of window before fullscreen.
         */
        int m_preFullscreenPosX = 0;

        /**
         * Y Position of window before fullscreen.
         */
        int m_preFullscreenPosY = 0;

        /**
         * Width of window before fullscreen.
         */
        int m_preFullscreenSizeWidth = 0;

        /**
         * Height of window before fullscreen.
         */
        int m_preFullscreenSizeHeight = 0;

#if defined(PLATFORM_DESKTOP) && defined(_WIN32)
        /**
         * Whether or not a native console is allocated
         */
        bool m_consoleAllocated = false;

        /**
         * Console input stream
         */
        std::fstream m_consoleIn;

        /**
         * Console output stream
         */
        std::fstream m_consoleOut;

        /**
         * Console error stream
         */
        std::fstream m_consoleErr;

        /**
         * Pointer to original input buffer
         */
        std::streambuf *m_oldCinBuffer;

        /**
         * Pointer to original output buffer
         */
        std::streambuf *m_oldCoutBuffer;

        /**
         * Pointer to original error buffer
         */
        std::streambuf *m_oldCerrBuffer;
#endif

        /**
         * Apply the provided window config.
         * This only happens when the window is created.
         */
        void _applyConfig(const WindowConfig &config_);
    public:
        /**
         * Create a new window.
         *
         * @param config_ The window config.
         */
        explicit Window(const WindowConfig &config_);
        ~Window();

        /**
         * Make this window's context current.
         */
        void MakeCurrent();

        /**
         * Get the current window.
         *
         * @return The currently active window.
         */
        static Window *GetCurrent();

        /**
         * Get window width.
         *
         * @return Current window width.
         */
        int GetWidth();

        /**
         * Get window height.
         *
         * @return Current window height.
         */
        int GetHeight();

        /**
         * Resize the window
         *
         * @param width_ New window width.
         * @param height_ New window height.
         */
        void Resize(int width_, int height_);

        /**
         * Set whether or not VSync is enabled
         *
         * @warning Makes this window's context current.
         * @param flag_
         */
        void SetVSyncEnabled(bool flag_);

        /**
         * Set the window icon.
         *
         * @param icon_ The new window icon.
         */
        void SetIcon(Graphics::Image *icon_);

        /**
         * Get whether or not the window is in fullscreen mode.
         *
         * @note This will still return false on platforms that don't support windowing, such as Xbox One or Android.
         * @return Whether the window is full screen nor not.
         */
        bool IsFullscreen();

        /**
         * Toggle fullscreen. Enables if disabled and vice versa.
         */
        void ToggleFullscreen();

        /**
         * Set fullscreen.
         *
         * @param fullscreen_ Whether the window should be fullscreen.
         */
        void SetFullscreen(bool fullscreen_);

        /**
         * Set whether or not the window can be resized.
         *
         * @note This can only be enforced on the Desktop platform.
         * @param resizable_ Whether the window can be resized.
         */
        void SetResizable(bool resizable_);

        /**
         * Get the window title.
         *
         * @warning This does not work yet.
         * @todo Get this working.
         */
        std::string GetTitle();

        /**
         * Set window title.
         *
         * @note Only works on Desktop platform.
         * @param title_ New window title.
         */
        void SetTitle(const std::string& title_);

        /**
         * Set whether or not there should be a native debug console.
         *
         * @note This only works on Windows native.
         * @param flag_ Whether or not the debug console should be attached.
         */
        void SetEnableNativeConsole(bool flag_);

        /**
         * Determine whether or not the window is focussed on.
         *
         * @note This only works on Desktop and UWP platforms.
         * @return Whether or not the window has focus.
         */
        bool IsFocussed();

        /**
         * Whether or not the window is visible.
         *
         * @return If the window is visible or not.
         */
        bool IsVisible();

        /**
         * Poll window events.
         */
        void PollEvents();

        /**
         * Close window.
         *
         * @warning This will delete this window's context.
         */
        void Close();

        /**
         * Determine Whether or not the window should close.
         * For example, the close button was pressed.
         *
         * @return Whether or not the window should close.
         */
        bool ShouldClose();

        /**
         * Swap the window buffers
         */
        void SwapBuffers();
    };
}

#endif //WINDOW_H
