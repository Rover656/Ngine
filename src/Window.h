#ifndef WINDOW_H
#define WINDOW_H

#include "ngine.h"

#if defined(PLATFORM_UWP)
namespace NerdThings::Ngine::UWP {
    ref class GameApp;
}

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;
#endif

#include "Graphics/Image.h"
#include "EventHandler.h"

namespace NerdThings::Ngine {
    /*
     * Window config.
     */
    struct TWindowConfig {
        /*
         * Fullscreen window.
         * TODO: This has not been implemented.
         */
        bool Fullscreen = false;

        /*
         * Window height
         */
        int Height = 0;

        /*
         * Window icon
         */
        Graphics::TImage *Icon = nullptr;

        /*
         * Enable MSAA 4X.
         * Cannot be changed after creation.
         */
        bool MSAA_4X = false;

        /*
         * Whether or not the window is resizable.
         */
        bool Resizable = false;

        /*
         * The window title
         */
        std::string Title = "Game Window";

        /*
         * Whether or not to use V-Sync
         */
        bool VSync = false;

        /*
         * Window width
         */
        int Width = 0;
    };

    /*
     * Ngine window management wrapper.
     */
    class NEAPI Window {
        // Private Fields

        /*
         * Current window height
         */
        static int _CurrentHeight;

        /*
         * Current window width
         */
        static int _CurrentWidth;

        /*
         * Whether or not the window is initialized
         */
        static bool _Initialized;

#if defined(PLATFORM_UWP)
        /*
         * When the application is suspended
         */
        static void Suspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
#endif
    public:
        // Public Fields

        /*
         * Window config
         */
        static TWindowConfig Config;

#if defined(PLATFORM_DESKTOP)
        /*
         * GLFW Window pointer
         * Most likely will be nullptr on non-desktop platforms
         */
        static void *WindowPtr;
#elif defined(PLATFORM_UWP)
        /*
         * EGL Context
         */
        static EGLContext Context;

        /*
         * EGL Display
         */
        static EGLDisplay Display;

        /*
         * EGL Surface
         */
        static EGLSurface Surface;
#endif

        // Public Methods

        /*
         * Apply changes in config
         */
        static void ApplyConfig();

        /*
         * Close window and clean API
         */
        static void Close();

        /*
         * Get window height
         */
        static int GetHeight();

        /*
         * Get window width
         */
        static int GetWidth();

        /*
         * Create a new window. Title ignored most non desktop platforms.
         */
        static void Init();

        /*
         * Poll window events
         */
        static void PollEvents();

        /*
         * Set window config.
         * Must be set before Initialization.
         */
        static void SetConfig(const TWindowConfig &config_);

        /*
         * Set fullscreen
         */
        static void SetFullscreen(bool fullscreen_);

        /*
         * Set whether or not the window can be resized (Desktop only)
         */
        static void SetResizable(bool resizable_);

        /*
         * Set window title (Ignored on most non desktop platforms.)
         */
        static void SetTitle(const std::string& title_);

        /*
         * Enable or disable vsync
         */
        static void SetVSync(bool vsync_);

        /*
         * Whether or not the window should be const closed
         */
        static bool ShouldClose();

        /*
         * Swap the window buffers
         */
        static void SwapBuffers();

        /*
         * Whether or not the window is visible.
         */
        static bool Visible();
    };
}

#endif //WINDOW_H
