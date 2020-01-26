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

#ifndef VERTEXLAYOUT_HPP
#define VERTEXLAYOUT_HPP

#include "../../Config.hpp"

#include "../GraphicsDevice.hpp"
#include "PlatformGraphicsAPI.hpp"

namespace Ngine::Graphics::API {
    /**
     * Description of what a vertex array contains and where it should be routed to in the shader.
     */
    class NEAPI VertexLayout {
    public:
        /**
         * Internal element data type.
         */
        enum class VertexElementType {
            Float
        };

        struct VertexElement {
            /**
             * The name of the element.
             */
            std::string ElementName;

            /**
             * The element index in the shader (OpenGL).
             */
            unsigned int ElementIndex;

            /**
             * The number of `Type` in the element.
             */
            int Count;

            /**
             * The data type used.
             */
            VertexElementType Type;

            /**
             * The data stride.
             */
            unsigned int Stride;

            /**
             * The data offset.
             */
            unsigned int Offset;

            /**
             * Is the data normalized?
             */
            bool Normalized = false;
        };
    private:
        /**
         * The graphics API.
         */
        PlatformGraphicsAPI *m_API = nullptr;

        /**
         * Once configured, the layout locks.
         */
        bool m_locked = false;

        /**
         * The elements defining this layout.
         */
        std::vector<VertexElement> m_elements;

        Buffer *m_vertexBuffer = nullptr;
        Buffer *m_indexBuffer = nullptr;

        /**
         * Track if we are being used.
         */
        bool m_using = false;
    public:
        union {
            /**
             * Vertex array object (OpenGL)
             */
            unsigned int VAO;

            /**
             * Pointer for platform use.
             */
            void *Ptr;
        };

        VertexLayout(GraphicsDevice *graphicsDevice_, Buffer *vertexBuffer_, Buffer *indexBuffer_);
        ~VertexLayout();

        /**
         * Add an element to the layout
         *
         * @warning Once `Configure` is called, this will throw as the layout cannot be modified at the moment.
         * @param element_
         */
        void AddElement(const VertexElement &element_);

        /**
         * Get the list of vertex elements.
         *
         * @return The list of vertex elements.
         */
        std::vector<VertexElement> GetElements();

        /**
         * Configure the vertex layout.
         * This should be done once all elements are added.
         */
        void Configure();

        Buffer *GetVertexBuffer();

        Buffer *GetIndexBuffer();

        /**
         * Use this layout.
         */
        void Use();

        /**
         * Stop using this layout.
         */
        void Stop();
    };
}

#endif //VERTEXLAYOUT_HPP
