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

#ifndef NGINE_GLFW_WINDOW_HPP
#define NGINE_GLFW_WINDOW_HPP

#include "ngine/config.hpp"

#if defined(PLATFORM_DESKTOP)

#include "ngine/window.hpp"

#include <GLFW/glfw3.h>

namespace ngine::platform {
    // For code cleanliness
    using namespace ngine;

    class GLFWWindow : public IWindow {
    public:
        /**
         * Create a GLFW window.
         */
        explicit GLFWWindow(WindowConfig config);

        /**
         * Destroy GLFW window.
         */
        ~GLFWWindow();

        void pollEvents() override;
        bool shouldClose() const override;
        void *getHandle() const override;

    private:
        /**
         * The GLFW window handle.
         */
        GLFWwindow *m_window;
    };
}

#endif // defined(PLATFORM_DESKTOP)

#endif //NGINE_GLFW_WINDOW_HPP
