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

#include "ngine/graphics/VertexDataConverter.hpp"

#include "ngine/Console.hpp"

#include <cmath>

namespace ngine::graphics {
    int VertexDataConverter::getTriangleIndices(PrimitiveType currentType,
                                                int vertexCount,
                                                unsigned short **indices) {
        switch (currentType) {
        case PrimitiveType::TriangleFan: {
            // Create array
            *indices = new unsigned short[(vertexCount - 1) * 3];

            for (auto i = 0; i + 2 < vertexCount; i++) {
                (*indices)[i * 3 + 0] = 0;
                (*indices)[i * 3 + 1] = i + 1;
                (*indices)[i * 3 + 2] = i + 2;
            }

            // Return count.
            return (vertexCount - 1) * 3;
        }
        case PrimitiveType::Quads: {
            // Create array
            *indices = new unsigned short[vertexCount / 4 * 6];

            // Generate indices
            for (auto i = 0; i < floor(vertexCount / 4); i++) {
                (*indices)[i * 6 + 0] = i * 4 + 0;
                (*indices)[i * 6 + 1] = i * 4 + 1;
                (*indices)[i * 6 + 2] = i * 4 + 3;
                (*indices)[i * 6 + 3] = i * 4 + 1;
                (*indices)[i * 6 + 4] = i * 4 + 2;
                (*indices)[i * 6 + 5] = i * 4 + 3;
            }

            // Return count
            return vertexCount / 4 * 6;
        }
        default:
            Console::fail("VertexDataTool", "Conversion cannot be completed.");
            return 0;
        }
    }
} // namespace ngine::graphics
