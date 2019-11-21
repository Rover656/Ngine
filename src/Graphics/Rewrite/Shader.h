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

#ifndef NEW_SHADER_H
#define NEW_SHADER_H

#include "../../Ngine.h"

#include "../../Resource.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    class ShaderProgram;

    class NEAPI Shader : public IResource {
        // Set ShaderProgram as a friend.
        friend class ShaderProgram;

        /**
         * Vertex shader ID.
         */
        unsigned int _VertexShader;

        /**
         * Fragment shader ID.
         */
        unsigned int _FragmentShader;

        /**
         * Whether or not the shader is compiled
         */
        bool _Compiled = false;

        /**
         * Compile the given shader.
         */
        bool __Compile(unsigned int id_);

    public:
        /**
         * Create a shader with source strings.
         *
         * @param vertSrc_
         * @param fragSrc_
         */
        Shader(std::string vertSrc_, std::string fragSrc_);
        ~Shader();

        /**
         * Compile the shader.
         *
         * @return Whether or not the shader compiled successfully.
         */
        bool Compile();

        /**
         * Determine if the shader is valid.
         *
         * @note If the shader has not compiled, or failed to compile it will not be valid.
         * @return Whether or not the shader is valid.
         */
        bool IsValid() const override;

        /**
         * Delete the shader.
         */
        void Unload() override;
    };
}

#endif //NEW_SHADER_H
