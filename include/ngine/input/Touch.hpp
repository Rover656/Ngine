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

#ifndef NGINE_TOUCH_HPP
#define NGINE_TOUCH_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Window.hpp"

namespace ngine::input {
    // TODO: This module needs to actually be implemented. This is just a layout
    // for the future API. Do not commit.

    /**
     * This declares a gesture type.
     */
    enum class GestureType : int {
        None = 0,
        Tap = 1,
        DoubleTap = 2,
        Hold = 4,
        Drag = 8,
        SwipeRight = 16,
        SwipeLeft = 32,
        SwipeUp = 64,
        SwipeDown = 128,
        PinchIn = 256,
        PinchOut = 512
    };

    class NEAPI TouchPanel {
        /**
         * The maximum number of touch points.
         */
        int m_maxTouchPoints = 1;

        /**
         * The window we are detecting touches in.
         */
        Window *m_window = nullptr;

        /**
         * Whether or not mouse events are forwarded as touch events.
         */
        bool m_mouseForTouch = true;

        float m_x;
        float m_y;
        float m_width;
        float m_height;

    public:
        /**
         * Get the number of touch points available to the panel.
         */
        int getTouchPointCount() const;

        /**
         * Get a touch position.
         *
         * @param point Touch point to get for.
         * @return The position of the touch.
         */
        Vector2 getTouchPos(int point) const;
    };
} // namespace ngine::input

#endif // NGINE_TOUCH_HPP
