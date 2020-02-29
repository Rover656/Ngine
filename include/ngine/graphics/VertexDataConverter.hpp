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

#ifndef VERTEXDATACONVERTER_HPP
#define VERTEXDATACONVERTER_HPP

#include "../Config.hpp"
#include "Renderer.hpp"

namespace ngine::graphics {
    /**
     * The vertex data converter converts other `PrimitiveType`'s to triangles, by creating indices for them.
     * One day I may add vertex optimisation...
     */
    class NEAPI VertexDataConverter {
    public:
        /**
         * Convert vertex data to triangles.
         *
         * @warning Indices must be deleted afterwards using delete[].
         */
        static int getTriangleIndices(PrimitiveType currentType, int vertexCount, unsigned short **indices);
    };
}

#endif //VERTEXDATACONVERTER_HPP
