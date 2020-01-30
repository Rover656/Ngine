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

#ifndef VERTEXDATATOOL_HPP
#define VERTEXDATATOOL_HPP

#include "../Config.hpp"
#include "Renderer.hpp"

namespace Ngine::Graphics {
    /**
     * The vertex data tool can do many useful things.
     * The tool can convert between primitive types (useful as the renderer only uses Triangles).
     * The tool can also generate vertices for basic things (such as a rect) and give you them in Triangle form.
     *
     * @warning This tool is a work in progress and may be removed at any time.
     * @todo Add a tool that can split vertex arrays that are too large?
     */
    class NEAPI VertexDataTool {
    public:
        /**
         * Generate a rectangle using indexed triangles.
         *
         * @param width_ Rectangle width.
         * @param height_ Rectangle height.
         * @param vertexArray_ The vertex array to write to.
         * @param indexArray_ The index array to write to.
         */
        static void GenerateRect(float width_, float height_, Color color_, std::vector<Vertex> &vertexArray_, std::vector<unsigned short> &indexArray_);

        /**
         * Convert vertex data to triangles.
         *
         * @warning Indices must be deleted afterwards using delete[].
         */
        static int GetTriangleIndices(PrimitiveType currentType_, int vertexCount_, unsigned short **indices_);
    };
}

#endif //VERTEXDATATOOL_HPP
