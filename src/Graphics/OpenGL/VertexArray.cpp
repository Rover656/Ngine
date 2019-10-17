/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "VertexArray.h"

// Platform specifics
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 // UWP???
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#endif

#include "OpenGL.h"

namespace NerdThings::Ngine::Graphics::OpenGL {
    ////////
    // GLVertexBufferElement
    ////////

    unsigned int GLVertexBufferElement::GetSizeOf(GLType type_) {
        switch (type_) {
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLubyte);
            default:
                throw std::runtime_error("Error, unsupported type.");
        }
    }

    ////////
    // GLVertexBufferLayout
    ////////

    void GLVertexBufferLayout::Clear() {
        _Stride = 0;
        _Elements.clear();
    }

    void GLVertexBufferLayout::Push(GLType type_, unsigned int count_, unsigned int location_) {
        unsigned int normalized = 0;
        if (type_ == TYPE_UNSIGNED_BYTE) normalized = 1;
        _Elements.push_back({type_, count_, normalized, location_});
        _Stride += GLVertexBufferElement::GetSizeOf(type_) * count_;
    }

    std::vector<GLVertexBufferElement> GLVertexBufferLayout::GetElements() const {
        return _Elements;
    }

    unsigned int GLVertexBufferLayout::GetStride() const {
        return _Stride;
    }

    ////////
    // GLVertexArray
    ////////

    GLVertexArray::GLVertexArray() {
        // Create vertex array
        GL::GenVertexArrays(1, &ID);
    }

    GLVertexArray::~GLVertexArray() {
        // Delete vertex array
        GL::DeleteVertexArrays(1, &ID);

        // Set ID to 0
        ID = 0;
    }

    void GLVertexArray::AddBuffer(std::shared_ptr<GLBuffer> vertexBuffer_, const GLVertexBufferLayout &layout_) {
        // Check the buffer is correct
        if (vertexBuffer_->GetBufferType() != BUFFER_VERTEX)
            throw std::runtime_error("A vertex buffer was not provided.");

        // Bind the array
        Bind();

        // Bind the buffer
        vertexBuffer_->Bind();

        // Init attribute pointers
        const auto elements = layout_.GetElements();
        unsigned int offset = 0;

        for (auto element : elements) {
            glEnableVertexAttribArray(element.Location);
            glVertexAttribPointer(element.Location, element.Count, element.Type, element.Normalized, layout_.GetStride(),
                                  (void *) offset);
            offset += element.Count * GLVertexBufferElement::GetSizeOf(element.Type);
        }
    }

    void GLVertexArray::Bind() {
        // Bind VAO
        GL::BindVertexArray(ID);
    }
}
#endif
