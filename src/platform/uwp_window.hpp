/**********************************************************************************************
 *
 *   Ngine - A game framework.
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

#ifndef NGINE_UWP_WINDOW_HPP
#define NGINE_UWP_WINDOW_HPP

#include "ngine/config.hpp"

#if defined(PLATFORM_UWP)

#include "ngine/utils/UWP/uwp_bootstrap.hpp"
#include "ngine/window.hpp"

namespace ngine::platform {
    using namespace ngine;

    class UWPWindow : public IWindow {
    public:
        /**
         * Create a UWP window.
         */
        explicit UWPWindow(WindowConfig config);

        void pollEvents() override;
        bool shouldClose() const override;
        void* getHandle() const override;

    private:
        /**
         * The UWP window.
         */
        CoreWindow ^m_window;
    };
}

#endif // defined(PLATFORM_UWP)

#endif //NGINE_UWP_WINDOW_HPP
