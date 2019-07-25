#ifndef SHADER_H
#define SHADER_H

#include "../../ngine.h"

namespace NerdThings::Ngine::Graphics::OpenGL {
    /*
     * OpenGL Shader type
     */
    enum GLShaderType {
        /*
         * Fragment shader.
         */
        SHADER_FRAGMENT = 0x8B30,

        /*
         * Vertex shader
         */
        SHADER_VERTEX = 0x8B31
    };

    /*
     * Shader, gets attached to a program
     */
    class GLShader {
        /*
         * Whether or not the shader has been compiled since the source was set/changed
         */
        bool _Dirty;

        /*
         * The shader type
         */
        GLShaderType _Type;
    public:
        /*
         * Shader ID
         */
        unsigned int ID;

        /*
         * Create a shader using a source string
         */
        GLShader(std::string src, GLShaderType type_);

        /*
         * Destruct and delete shader
         */
        ~GLShader();

        /*
         * Compile the shader
         */
        bool Compile();

        /*
         * Get the shader type
         */
        GLShaderType GetShaderType();

        /*
         * Get whether or not the shader is dirty.
         * If false the shader can be used safely.
         */
        bool IsDirty();

        /*
         * Update the shader source
         */
        void SetSource(std::string src);
    };

    /*
     * OpenGL Shader Locations.
     * Used by GL class.
     */
    enum GLShaderLocation {
        LOCATION_VERTEX_POSITION = 0,
        LOCATION_VERTEX_TEXCOORD,
        LOCATION_VERTEX_COLOR,
        LOCATION_MATRIX_MVP,
        LOCATION_TEXTURE
    };

    /*
     * GL shader attribute.
     * Used as the Attribs array index.
     */
    enum GLShaderAttribute {
        ATTRIB_POSITION = 0,
        ATTRIB_TEXCOORD,
        ATTRIB_COLOR
    };

    /*
     * GL shader uniform.
     * Used as the Uniforms array index.
     */
    enum GLShaderUniform {
        UNIFORM_MATRIX_MVP = 0,
        UNIFORM_TEXTURE
    };

    /*
     * OpenGL Shader program.
     * Uses a fragment and a vertex shader.
     */
    class GLShaderProgram {
        /*
         * Whether or not the shader program is dirty
         */
        bool _Dirty;
    public:
        /*
         * Shader attribute names
         */
        const char *Attribs[ATTRIB_COLOR + 1];

        /*
         * Shader locations.
         * Used by GL class.
         */
        unsigned int Locations[LOCATION_TEXTURE + 1];

        /*
         * Shader uniform names
         */
        const char *Uniforms[UNIFORM_TEXTURE + 1];

        /*
         * Shader program ID
         */
        unsigned int ID;

        /*
         * Create a GL shader program with two shaders
         */
        GLShaderProgram(std::shared_ptr<GLShader> fragmentShader_, std::shared_ptr<GLShader> vertexShader_, bool doLink_ = true);

        /*
         * Destruct and delete shader program
         */
        ~GLShaderProgram();

        /*
         * Attach a shader to the program for linking
         */
        void AttachShader(std::shared_ptr<GLShader> shader_);

        /*
         * Detach a shader from the program after linking
         */
        void DetachShader(std::shared_ptr<GLShader> shader_);

        /*
         * Whether or not the shader program is dirty
         */
        bool IsDirty();

        /*
         * Link the shader
         */
        bool Link();

        /*
         * Use the shader program
         */
        void Use();
    };
}

#endif //SHADER_H
