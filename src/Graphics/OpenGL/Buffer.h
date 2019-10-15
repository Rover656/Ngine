#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#ifndef BUFFER_H
#define BUFFER_H

#include "../../Ngine.h"

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
