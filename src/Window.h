#ifndef WINDOW_H
#define WINDOW_H

#include "ngine.h"

#if defined(PLATFORM_UWP)
namespace NerdThings::Ngine::UWP {
    ref class GameApp;
}
#endif

namespace NerdThings::Ngine {
    /*
     * Ngine window management wrapper.
     */
    class NEAPI Window {
        // Private Fields

        /*
         * Window height
         */
        static int _Height;

        /*
         * Window width
         */
        static int _Width;

#if defined(PLATFORM_UWP)
        /*
         * When the application is suspended
         */
        static void Suspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
#endif
    public:
        // Public Fields

        /*
         * Platform specific window pointer.
         * Most likely will be nullptr on non-desktop platforms
         */
        static void *WindowPtr;

#if defined(PLATFORM_UWP)
        static UWP::GameApp ^UWPApp;
#endif

        // Public Methods

        /*
         * Close window and clean API
         */
        static void Cleanup();

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
        static void Init(int width_, int height_, const std::string& title_);

        /*
         * Poll window events
         */
        static void PollEvents();

        /*
         * Set whether or not the window can be resized (Desktop only)
         */
        static void SetResizable(bool resizable_);

        /*
         * Set window title (Ignored on most non desktop platforms.)
         */
        static void SetTitle(const std::string& title_);

        /*
         * Whether or not the window should be const closed
    &     */
        static bool ShouldClose();

        /*
         * Whether or not to run this frame
         */
        static bool ShouldRenderFrame();

        /*
         * Swap the window buffers
         */
        static void SwapBuffers();
    };
}

#endif //WINDOW_H
