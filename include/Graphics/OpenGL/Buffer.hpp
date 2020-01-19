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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#ifndef BUFFER_H
#define BUFFER_H

#include "../../Config.hpp"

namespace NerdThings::Ngine::Graphics::OpenGL {
    enum GLBufferType {
        /*
         * Vertex buffer
         */
        BUFFER_VERTEX = 0x8892,

        /*
         * Index buffer
         */
        BUFFER_INDEX = 0x8893
    };

    enum GLBufferUsage {
        /*
         * Static use
         */
        USAGE_STATIC = 0x88E4,

        /*
         * Dynamic usa
         */
        USAGE_DYNAMIC = 0x88E8
    };

    /*
     * OpenGL Data Buffer
     */
    class NEAPI GLBuffer {
        /*
         * The buffer type
         */
        GLBufferType _Type;

        /*
         * The buffer usage
         */
        GLBufferUsage _Usage;
    public:
        /*
         * The Buffer ID
         */
        unsigned int ID;

        /*
         * Create a new GL Buffer
         */
        GLBuffer(GLBufferType type_, GLBufferUsage usage_ = USAGE_DYNAMIC);

        /*
         * Destruct and delete buffer
         */
        ~GLBuffer();

        /*
         * Bind buffer
         */
        void Bind();

        /*
         * Get the buffer type
         */
        GLBufferType GetBufferType();

        /*
         * Get the buffer usage
         */
        GLBufferUsage GetBufferUsage();

        /*
         * Set the buffer data
         */
        void SetData(void *data_, int dataSize_);
    };
}

#endif //BUFFER_H
#endif
