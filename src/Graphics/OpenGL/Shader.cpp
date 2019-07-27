#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL33)
#include "Shader.h"

// Platform specifics
#if defined(GRAPHICS_OPENGL33)
    #include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
#endif

#if defined(PLATFORM_DESKTOP)
    #include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
    #include <angle_windowsstore.h>
#endif

namespace NerdThings::Ngine::Graphics::OpenGL {
    ////////
    // GLShader
    ////////

    GLShader::GLShader(std::string src, GLShaderType type_) : _Dirty(true), _Type(type_) {
        // Create shader
        ID = glCreateShader(_Type);

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
        glCompileShader(ID);

        // Check compile status
        int success = GL_TRUE;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE)
            ConsoleMessage("Failed to compile shader.", "WARN", "GLShader");

        // Mark as clean
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

        // Set default attribs
        Attribs[ATTRIB_POSITION] = "vertexPosition";
        Attribs[ATTRIB_TEXCOORD] = "vertexTexCoord";
        Attribs[ATTRIB_COLOR] = "vertexColor";

        // Set default uniforms
        Uniforms[UNIFORM_MATRIX_MVP] = "mvp";
        Uniforms[UNIFORM_TEXTURE] = "texture";

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

        // Set ID to 0
        ID = 0;
    }

    void GLShaderProgram::AttachShader(std::shared_ptr<GLShader> shader_) {
        // Check if it is dirty
        if (shader_->IsDirty())
            throw std::runtime_error("Cannot attach a dirty shader to a shader program.");

        // Attach shader
        glAttachShader(ID, shader_->ID);

        // Mark as dirty
        _Dirty = true;
    }

    void GLShaderProgram::DetachShader(std::shared_ptr<GLShader> shader_) {
        // Detach shader
        glDetachShader(ID, shader_->ID);
    }

    bool GLShaderProgram::IsDirty() {
        return _Dirty;
    }

    bool GLShaderProgram::Link() {
        // Link program
        glLinkProgram(ID);

        // Check link success
        int linked = GL_TRUE;
        glGetProgramiv(ID, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE)
            ConsoleMessage("Failed to link shader program.", "WARN", "GLShaderProgram");

        // Get locations
        Locations[LOCATION_VERTEX_POSITION] = glGetAttribLocation(ID, Attribs[ATTRIB_POSITION]);
        Locations[LOCATION_VERTEX_TEXCOORD] = glGetAttribLocation(ID, Attribs[ATTRIB_TEXCOORD]);
        Locations[LOCATION_VERTEX_COLOR] = glGetAttribLocation(ID, Attribs[ATTRIB_COLOR]);
        Locations[LOCATION_MATRIX_MVP] = glGetUniformLocation(ID, Uniforms[UNIFORM_MATRIX_MVP]);
        Locations[LOCATION_TEXTURE] = glGetUniformLocation(ID, Uniforms[UNIFORM_TEXTURE]);

        return linked == GL_TRUE;
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
