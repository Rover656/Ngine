/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "Shader.hpp"

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

#include "../../Logger.hpp"

namespace NerdThings::Ngine::Graphics::OpenGL {
    ////////
    // GLShader
    ////////

    GLShader::GLShader(std::string src, GLShaderType type_) : _Dirty(true), _Type(type_) {
        // Create shader
        ID = glCreateShader(_Type);
        Logger::Notice("GLShader", "Created new shader with ID %i.", ID);

        // Set source
        SetSource(std::move(src));

        // Try to compile
        _Dirty = !Compile();
    }

    GLShader::~GLShader() {
        // Delete
        glDeleteShader(ID);

        // Set ID to 0
        ID = 0;
    }

    bool GLShader::Compile() {
        // Compile shader
        Logger::Notice("GLShader", "Attempting to compile shader with ID %i.", ID);
        glCompileShader(ID);

        // Check compile status
        int success = GL_TRUE;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE) Logger::Warn("GLShader", "Failed to compile shader.");
        else Logger::Notice("GLShader", "Compiled shader successfully.");

        // Mark as clean if successful
        _Dirty = !success;

        // Return the success or fail
        return success == GL_TRUE;
    }

    GLShaderType GLShader::GetShaderType() {
        return _Type;
    }

    bool GLShader::IsDirty() {
        return _Dirty;
    }

    void GLShader::SetSource(std::string src) {
        // Set source
        const char *srcStr = src.c_str();
        glShaderSource(ID, 1, &srcStr, nullptr);

        // Mark as dirty
        _Dirty = true;
    }

    ////////
    // GLShaderProgram
    ////////

    GLShaderProgram::GLShaderProgram(std::shared_ptr<GLShader> fragmentShader_, std::shared_ptr<GLShader> vertexShader_,
                                     bool doLink_) {
        // Validate inputs
        if (fragmentShader_->GetShaderType() != SHADER_FRAGMENT)
            throw std::runtime_error("A fragment shader was not provided.");
        if (vertexShader_->GetShaderType() != SHADER_VERTEX)
            throw std::runtime_error("A vertex shader was not provided.");

        // Create program
        ID = glCreateProgram();
        Logger::Notice("GLShaderProgram", "Created a new shader program with ID %i.", ID);

        // Set default attribs
        Attribs[ATTRIB_POSITION] = "vertexPosition";
        Attribs[ATTRIB_TEXCOORD] = "vertexTexCoord";
        Attribs[ATTRIB_COLOR] = "vertexColor";

        // Set default uniforms
        Uniforms[UNIFORM_MATRIX_MVP] = "mvp";
        Uniforms[UNIFORM_TEXTURE] = "texture";
        Logger::Notice("GLShaderProgram", "Loaded default attribute and uniform names.");

        // Perform link
        // We give the option to not if the above params need tweaks
        if (doLink_) {
            // Attach shaders
            AttachShader(fragmentShader_);
            AttachShader(vertexShader_);

            // Link
            _Dirty = !Link();

            // Detach
            DetachShader(fragmentShader_);
            DetachShader(vertexShader_);
        }
    }

    GLShaderProgram::~GLShaderProgram() {
        // Delete program
        glDeleteProgram(ID);
        Logger::Notice("GLShaderProgram", "Deleting shader program with ID %i.", ID);

        // Set ID to 0
        ID = 0;
    }

    void GLShaderProgram::AttachShader(std::shared_ptr<GLShader> shader_) {
        // Check if it is dirty
        if (shader_->IsDirty())
            throw std::runtime_error("Cannot attach a dirty shader to a shader program.");

        // Attach shader
        glAttachShader(ID, shader_->ID);
        Logger::Notice("GLShaderProgram", "Attached shader with ID %i to shader program with ID %i.", shader_->ID, ID);

        // Mark as dirty
        _Dirty = true;
    }

    void GLShaderProgram::DetachShader(std::shared_ptr<GLShader> shader_) {
        // Detach shader
        glDetachShader(ID, shader_->ID);
        Logger::Notice("GLShaderProgram", "Detached shader with ID %i from shader program with ID %i.", shader_->ID, ID);
    }

    unsigned int GLShaderProgram::GetAttributeLocation(const std::string &name_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot get attribute location on shader program that is not linked.");

        // Get location
        return glGetAttribLocation(ID, name_.c_str());
    }

    unsigned int GLShaderProgram::GetUniformLocation(const std::string &name_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot get attribute location on shader program that is not linked.");

        // Get location
        return glGetUniformLocation(ID, name_.c_str());
    }

    bool GLShaderProgram::IsDirty() {
        return _Dirty;
    }

    bool GLShaderProgram::IsLinked() {
        return _Linked;
    }

    bool GLShaderProgram::Link() {
        // Link program
        Logger::Notice("GLShaderProgram", "Attempting to link shader program with ID %i.", ID);
        glLinkProgram(ID);

        // Check link success
        int linked = GL_TRUE;
        glGetProgramiv(ID, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE) Logger::Warn("GLShaderProgram", "Failed to link shader program.");
        else Logger::Notice("GLShaderProgram", "Linked shader program successfully.");

        // Mark as linked
        _Linked = linked == GL_TRUE;

        // Get locations
        if (IsLinked()) {
            Locations[LOCATION_VERTEX_POSITION] = GetAttributeLocation(Attribs[ATTRIB_POSITION]);
            Locations[LOCATION_VERTEX_TEXCOORD] = GetAttributeLocation(Attribs[ATTRIB_TEXCOORD]);
            Locations[LOCATION_VERTEX_COLOR] = GetAttributeLocation(Attribs[ATTRIB_COLOR]);
            Locations[LOCATION_MATRIX_MVP] = GetUniformLocation(Uniforms[UNIFORM_MATRIX_MVP]);
            Locations[LOCATION_TEXTURE] = GetUniformLocation(Uniforms[UNIFORM_TEXTURE]);
            Logger::Notice("GLShaderProgram", "Collecting all locations.");
        }

        return IsLinked();
    }

    void GLShaderProgram::SetUniformFloat(unsigned int loc_, float float_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform1f(loc_, float_);
    }

    void GLShaderProgram::SetUniformFloat(unsigned int loc_, float float1_, float float2_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform2f(loc_, float1_, float2_);
    }

    void GLShaderProgram::SetUniformFloat(unsigned int loc_, float float1_, float float2_, float float3_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform3f(loc_, float1_, float2_, float3_);
    }

    void GLShaderProgram::SetUniformFloat(unsigned int loc_, float float1_, float float2_, float float3_, float float4_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform4f(loc_, float1_, float2_, float3_, float4_);
    }

    void GLShaderProgram::SetUniformInt(unsigned int loc_, int int_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform1i(loc_, int_);
    }

    void GLShaderProgram::SetUniformInt(unsigned int loc_, int int1_, int int2_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform2i(loc_, int1_, int2_);
    }

    void GLShaderProgram::SetUniformInt(unsigned int loc_, int int1_, int int2_, int int3_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform3i(loc_, int1_, int2_, int3_);
    }

    void GLShaderProgram::SetUniformInt(unsigned int loc_, int int1_, int int2_, int int3_, int int4_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform4i(loc_, int1_, int2_, int3_, int4_);
    }

    void GLShaderProgram::SetUniformMatrix(unsigned int loc_, const Matrix &matrix_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniformMatrix4fv(loc_, 1, false, matrix_.ToFloatArray().data());
    }

    void GLShaderProgram::SetUniformUInt(unsigned int loc_, unsigned int int_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        // TODO: This dont work on ANGLE
#if defined(GRAPHICS_OPENGL33)
        glUniform1ui(loc_, int_);
#endif
    }

    void GLShaderProgram::SetUniformUInt(unsigned int loc_, unsigned int int1_, unsigned int int2_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform2i(loc_, int1_, int2_);
    }

    void GLShaderProgram::SetUniformUInt(unsigned int loc_, unsigned int int1_, unsigned int int2_, unsigned int int3_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform3i(loc_, int1_, int2_, int3_);
    }

    void GLShaderProgram::SetUniformUInt(unsigned int loc_, unsigned int int1_, unsigned int int2_, unsigned int int3_,
                                         unsigned int int4_) {
        // No unlinked shaders
        if (!IsLinked()) throw std::runtime_error("Cannot set data inside unlinked shader programs.");

        // Set value
        glUniform4i(loc_, int1_, int2_, int3_, int4_);
    }

    void GLShaderProgram::SetUniformVector2(unsigned int loc_, Vector2 vec_) {
        SetUniformFloat(loc_, vec_.X, vec_.Y);
    }

    void GLShaderProgram::SetUniformVector3(unsigned int loc_, Vector3 vec_) {
        SetUniformFloat(loc_, vec_.X, vec_.Y, vec_.Z);
    }

    void GLShaderProgram::Use() {
        // Check if dirty
        if (_Dirty)
            throw std::runtime_error("Cannot use a dirty shader program.");

        // Use shader program
        glUseProgram(ID);
    }
}
#endif
