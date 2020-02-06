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

#ifndef BOUNDINGBOX2D_H
#define BOUNDINGBOX2D_H

#include "../Config.hpp"

#include "../Math.hpp"
#include "Shape.hpp"

namespace ngine::physics {
    /*
     * A 2D Bounding Box
     */
    struct NEAPI BoundingBox : public Shape {
        /*
         * Maximum coordinate
         */
        Vector2 Max;

        /*
         * Minimum coordinate
         */
        Vector2 Min;

        /*
         * Create a default bounding box
         */
        BoundingBox()
            : Max({}), Min({}) {}
    };
}

#endif
