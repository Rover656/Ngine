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

#include "Graphics/VertexDataTool.hpp"

#include "Console.hpp"

namespace Ngine::Graphics {
    void VertexDataTool::GenerateRect(float width_, float height_, Color color_, std::vector<Vertex> &vertexArray_,
                                      std::vector<unsigned short> &indexArray_) {
        auto vertStart = vertexArray_.size();
        vertexArray_.push_back({{0, 0, 0},
                                {0, 0},
                                color_});
        vertexArray_.push_back({{0, height_, 0},
                                {0, 1},
                                color_});
        vertexArray_.push_back({{width_, height_, 0},
                                {1, 1},
                                color_});
        vertexArray_.push_back({{width_, 0, 0},
                                {1, 0},
                                color_});

        indexArray_.push_back(vertStart + 0);
        indexArray_.push_back(vertStart + 1);
        indexArray_.push_back(vertStart + 3);
        indexArray_.push_back(vertStart + 1);
        indexArray_.push_back(vertStart + 2);
        indexArray_.push_back(vertStart + 3);
    }

    int VertexDataTool::GetTriangleIndices(PrimitiveType currentType_, int vertexCount_, unsigned short **indices_) {
        switch (currentType_) {
            case PrimitiveType::TriangleFan: {
                // Create array
                *indices_ = new unsigned short[(vertexCount_ - 1) * 3];

                for (auto i = 0; i + 2 < vertexCount_; i++) {
                    (*indices_)[i * 3 + 0] = 0;
                    (*indices_)[i * 3 + 1] = i + 1;
                    (*indices_)[i * 3 + 2] = i + 2;
                }

                // Return count.
                return (vertexCount_ - 1) * 3;
            }
            case PrimitiveType::Quads: {
                // Create array
                *indices_ = new unsigned short[vertexCount_ / 4 * 6];

                // Generate indices
                for (auto i = 0; i < floor(vertexCount_ / 4); i++) {
                    (*indices_)[i * 6 + 0] = i * 4 + 0;
                    (*indices_)[i * 6 + 1] = i * 4 + 1;
                    (*indices_)[i * 6 + 2] = i * 4 + 3;
                    (*indices_)[i * 6 + 3] = i * 4 + 1;
                    (*indices_)[i * 6 + 4] = i * 4 + 2;
                    (*indices_)[i * 6 + 5] = i * 4 + 3;
                }

                // Return count
                return vertexCount_ / 4 * 6;
            }
            default:
                Console::Fail("VertexDataTool", "Conversion cannot be completed.");
                return 0;
        }
    }
}