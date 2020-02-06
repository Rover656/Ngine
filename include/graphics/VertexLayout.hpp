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

#include "../Config.hpp"

#include "API/PlatformGraphicsAPI.hpp"
#include "Buffer.hpp"
#include "GraphicsDevice.hpp"

namespace ngine::graphics {
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
        API::PlatformGraphicsAPI *m_API = nullptr;

        /**
         * Once configured, the layout locks.
         */
        bool m_locked = false;

        /**
         * The elements defining this layout.
         */
        std::vector<VertexElement> m_elements;

        /**
         * The attached vertex buffer.
         */
        Buffer *m_vertexBuffer = nullptr;

        /**
         * The attached index buffer.
         */
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

        /**
         * Create a vertex layout.
         *
         * @param graphicsDevice_ The graphics device.
         * @param vertexBuffer_ The vertex buffer to bind when using this layout.
         * @param indexBuffer_ The index buffer to bind when using this layout or null.
         */
        VertexLayout(GraphicsDevice *graphicsDevice_, Buffer *vertexBuffer_, Buffer *indexBuffer_);
        ~VertexLayout();

        /**
         * Add an element to the layout
         *
         * @warning Once `Configure` is called, this will throw as the layout cannot be modified at the moment.
         * @param element_
         */
        void addElement(const VertexElement &element_);

        /**
         * Get the list of vertex elements.
         *
         * @return The list of vertex elements.
         */
        std::vector<VertexElement> getElements();

        /**
         * Configure the vertex layout.
         * This should be done once all elements are added.
         */
        void configure();

        /**
         * Get the attached vertex buffer.
         */
        Buffer *getVertexBuffer();

        /**
         * Get the attached index buffer.
         */
        Buffer *getIndexBuffer();

        /**
         * Use this layout.
         */
        void use();

        /**
         * Stop using this layout.
         */
        void stop();
    };
}

#endif //VERTEXLAYOUT_HPP
