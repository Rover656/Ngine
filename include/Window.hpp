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

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Config.hpp"

#if defined(PLATFORM_DESKTOP)
typedef struct GLFWwindow GLFWwindow;
#elif defined(PLATFORM_UWP)
namespace ngine::UWP {
    ref class GameApp;
}

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;
#endif

#include <fstream>
#include <iostream>

#include "graphics/API/PlatformGraphicsAPI.hpp"
#include "graphics/Image.hpp"
#include "Events.hpp"

namespace ngine {
    namespace graphics {
        class GraphicsDevice;
    }

    namespace input {
        class Mouse;

        class Keyboard;
    }

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
        graphics::Image *Icon = nullptr;

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

        /**
         * The target graphics API for the window context.
         */
        graphics::GraphicsAPI TargetAPI;

        /**
         * Target major API version.
         *
         * Set to -1 for default.
         */
        int TargetAPIMajorVersion = -1;

        /**
         * Target minor API version.
         *
         * Set to -1 for default.
         */
        int TargetAPIMinorVersion = -1;
    };

    /**
     * Ngine window management wrapper.
     */
    class NEAPI Window {
        friend class input::Mouse;

        friend class input::Keyboard;

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
         * UWP Window count.
         */
        static int m_UWPWindowCount;

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

        /**
         * Whether or not the window is focussed.
         */
        bool m_focussed = false;

        /**
         * Whether or not the window is visible.
         */
        bool m_visible = false;

        /**
         * Fullscreen flag
         * 0 - no change
         * 1 - fullscreen
         * 2 - exit fullscreen
         */
        char m_fullscreenSet = 0;
#endif

        /**
         * The config the window was created with.
         */
        const WindowConfig m_creationConfig;

        /**
         * This windows context API type.
         */
        graphics::GraphicsAPI m_targetAPI;

        /**
         * This windows context API major version.
         */
        int m_targetMajor;

        /**
         * This windows context API minor version.
         */
        int m_targetMinor;

        /**
         * The mouse input manager for this window.
         */
        input::Mouse *m_mouseInput = nullptr;

        /**
         * The keyboard input manager for this window.
         */
        input::Keyboard *m_keyboardInput = nullptr;

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

        /**
         * The window title.
         */
        std::string m_windowTitle = "";

        /**
         * The graphics device attached to the window context.
         */
        graphics::GraphicsDevice *m_graphicsDevice;

        /**
         * This window's viewport.
         */
        graphics::Viewport m_windowViewport;

        /**
         * Whether or not Vsync is currently enabled.
         */
        bool m_vsync = false;

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
        void _applyConfig(const WindowConfig &config);

    public:
        /**
         * Create a new window.
         *
         * @param config The window config.
         */
        explicit Window(const WindowConfig &config);
        ~Window();

        /**
         * Get the config as it was when the window was created.
         *
         * @return The creation config.
         */
        const WindowConfig getCreationConfig() const;

        /**
         * Get the API our context is targeting.
         *
         * @return The context API.
         */
        graphics::GraphicsAPI getContextAPI() const;

        /**
         * Get the API major version our context is targeting.
         *
         * @return The context major version.
         */
        int getContextAPIMajorVersion() const;

        /**
         * Get the API minor version our context is targeting.
         *
         * @return The context minor version.
         */
        int getContextAPIMinorVersion() const;

        /**
         * Get the graphics device for the window context.
         *
         * @return The graphics device.
         */
        graphics::GraphicsDevice *getGraphicsDevice();

        /**
         * Make this window's context current.
         */
        void makeCurrent();

        /**
         * Get the current window.
         *
         * @return The currently active window.
         */
        static Window *GetCurrent();

        /**
         * Get the mouse input handler.
         */
        input::Mouse *getMouse();

        /**
         * Get the keyboard input handler.
         */
        input::Keyboard *getKeyboard();

        /**
         * Get window width.
         *
         * @return Current window width.
         */
        int getWidth();

        /**
         * Get window height.
         *
         * @return Current window height.
         */
        int getHeight();

        /**
         * Resize the window
         *
         * @param width New window width.
         * @param height New window height.
         */
        void setSize(int width, int height);

        /**
         * Get this window's viewport.
         *
         * @return The window viewport.
         */
        const graphics::Viewport *getWindowViewport();

        /**
         * Get whether or not VSync has been enabled.
         *
         * @warning Will not check if VSync is supported.
         * @return Whether or not VSync has been enabled.
         */
        bool getVSyncEnabled();

        /**
         * Set whether or not VSync is enabled
         *
         * @warning Makes this window's context current.
         * @param flag Whether or not V-Sync should be enabled.
         */
        void setVSyncEnabled(bool flag);

        /**
         * Set the window icon.
         *
         * @param icon The new window icon.
         */
        void setIcon(graphics::Image *icon);

        /**
         * Get whether or not the window is in fullscreen mode.
         *
         * @note This will still return false on platforms that don't support windowing, such as Xbox One or Android.
         * @return Whether the window is full screen nor not.
         */
        bool isFullscreen();

        /**
         * Toggle fullscreen. Enables if disabled and vice versa.
         */
        void toggleFullscreen();

        /**
         * Set fullscreen.
         *
         * @param fullscreen Whether the window should be fullscreen.
         */
        void setFullscreen(bool fullscreen);

        /**
         * Set whether or not the window can be resized.
         *
         * @note This can only be enforced on the Desktop platform.
         * @param resizable Whether the window can be resized.
         */
        void setResizable(bool resizable);

        /**
         * Get the window title.
         *
         * @note Only works on Desktop and UWP.
         * @return The window title (if available.)
         */
        std::string getTitle();

        /**
         * Set window title.
         *
         * @note Only works on Desktop platform.
         * @param title New window title.
         */
        void setTitle(const std::string &title);

        /**
         * Set whether or not there should be a native debug console.
         *
         * @note This only works on Windows native.
         * @param flag Whether or not the debug console should be attached.
         */
        void setEnableNativeConsole(bool flag);

        /**
         * Determine whether or not the window is focussed on.
         *
         * @note This only works on Desktop and UWP platforms.
         * @return Whether or not the window has focus.
         */
        bool isFocussed();

        /**
         * Whether or not the window is visible.
         *
         * @return If the window is visible or not.
         */
        bool isVisible();

        /**
         * Poll window events.
         */
        void pollEvents();

        /**
         * Close window.
         *
         * @warning This will delete this window's context.
         */
        void close();

        /**
         * Determine Whether or not the window should close.
         * For example, the close button was pressed.
         *
         * @return Whether or not the window should close.
         */
        bool shouldClose();

        /**
         * Swap the window buffers
         */
        void swapBuffers();
    };
}

#endif //WINDOW_HPP
