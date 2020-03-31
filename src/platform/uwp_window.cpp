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

#include "uwp_window.hpp"

#if defined(PLATFORM_UWP)

namespace ngine::platform {
    UWPWindow::UWPWindow(WindowConfig config) : IWindow(config) {
        // Verify context descriptor
        _verifyContextDescriptor();

        // Get the window
        m_window = CoreWindow::GetForCurrentThread();
        
        // Get initial size
        auto bounds = m_window->Bounds;
        m_windowWidth = bounds.Width;
        m_windowHeight = bounds.Height;

        // Gather remaining information
        m_windowCount++;
        m_title = config.Title;

        // Create graphics device
        _createGraphicsDevice();
    }

    void UWPWindow::pollEvents() {
        // Poll window events
        if (m_window->Visible == true) // TODO: Visible field
            m_window->Dispatcher->ProcessEvents(
                CoreProcessEventsOption::ProcessAllIfPresent);
        else
            m_window->Dispatcher->ProcessEvents(
                CoreProcessEventsOption::ProcessOneAndAllPending);

        auto bounds = m_window->Bounds;
        m_windowWidth = bounds.Width;
        m_windowHeight = bounds.Height;
    }

    bool UWPWindow::shouldClose() const {
        // Handled by the bootstrap atm.
        return false;
    }

    void *UWPWindow::getHandle() const {
        return nullptr;
    }
}

#endif // defined(PLATFORM_UWP)
