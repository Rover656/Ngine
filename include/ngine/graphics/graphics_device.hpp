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

#ifndef NGINE_GRAPHICSDEVICE_HPP
#define NGINE_GRAPHICSDEVICE_HPP

#include "ngine/config.hpp"

namespace ngine {
    class Window;

    namespace graphics {
        class NEAPI GraphicsDevice {
            friend class Window;
        public:
        private:
            /**
             * Create a new graphics device.
             * @param window Window whose context we will use.
             */
            GraphicsDevice(Window *window);

            /**
             * Destroy the graphics device.
             */
            ~GraphicsDevice();

            /**
             * The window whose context we utilise.
             */
            Window *m_context;
        };
    }
}

#endif //NGINE_GRAPHICSDEVICE_HPP
