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

#ifndef PHYSICSCONTEXT_HPP
#define PHYSICSCONTEXT_HPP

#include "../Config.hpp"

namespace ngine::physics {
    /**
     * The physics context defines how units are handled in the underlying physics engine.
     */
    class NEAPI PhysicsContext {
        /**
         * Pixels per meter unit.
         */
        const int m_pixelsPerMeter;
    public:
        /**
         * Create a new physics context.
         *
         * @param ppm Pixels per meter.
         */
        PhysicsContext(int ppm);

        /**
         * Convert a dimension in pixels to meters.
         *
         * @tparam T Type to convert.
         * @param pixels Dimension in pixels.
         * @return The dimension in meters.
         */
        template <typename T>
        T convertPixelsToMeters(T pixels) const {
            return pixels / m_pixelsPerMeter;
        }

        /**
         * Convert a dimension in meters to pixels.
         *
         * @tparam T Type to convert.
         * @param pixels Dimension in meters.
         * @return The dimension in pixels.
         */
        template <typename T>
        T convertMetersToPixels(T meters) const {
            return meters * m_pixelsPerMeter;
        }
    };
}

#endif //PHYSICSCONTEXT_HPP
