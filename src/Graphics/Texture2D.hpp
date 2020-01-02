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

#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include "../Config.hpp"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Texture.hpp"
#endif

#include "../Filesystem/Filesystem.hpp"
#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "../Resource.hpp"
#include "Color.hpp"
#include "Image.hpp"

namespace NerdThings::Ngine::Graphics {
    class GraphicsDevice;
    class Renderer;

    /**
     * Texture filter mode.
     */
    enum TextureFilterMode {
        /**
         * No filter, just approximation.
         */
        FILTER_POINT = 0,

        /**
         * Linear filtering.
         */
        FILTER_BILINEAR,

        /**
         * Trilinear filtering (with mipmaps).
         */
        FILTER_TRILINEAR,

        /**
         * Anisotropic filtering 4x.
         */
        FILTER_ANISOTROPIC_4X,

        /**
         * Anisotropic filtering 8x.
         */
        FILTER_ANISOTROPIC_8X,

        /**
         * Anisotropic filtering 16x.
         */
        FILTER_ANISOTROPIC_16X
    };

    /**
     * Texture wrap mode.
     */
    enum TextureWrapMode {
        /**
         * Repeats texture
         */
        WRAP_REPEAT = 0,

        /**
         * Clamps texture to edge pixel.
         */
        WRAP_CLAMP,

        /**
         * Mirrors and repeats the texture.
         */
        WRAP_MIRROR_REPEAT,

        /**
         * Mirrors and clamps to the border.
         */
        WRAP_MIRROR_CLAMP
    };

    /**
     * A 2D Texture stored in the GPU memory.
     */
    class NEAPI Texture2D : public IResource {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
        // Friend renderer
        friend class Renderer;

        /**
         * OpenGL Texture ID
         */
        unsigned int m_ID = 0;

        /**
         * The number of mipmaps.
         */
        unsigned int m_mipmapCount = 0;

        /**
         * The pixel format.
         */
        PixelFormat m_format = UNCOMPRESSED_R8G8B8A8;

        /**
         * Create the texture on the GPU
         *
         * @param data_ The pixel data.
         */
        void _createTexture(GraphicsDevice *graphicsDevice_, unsigned char *data_);

        /**
         * Calculate the size of my pixel data.
         */
        int _calculatePixelDataSize();
#endif
#endif
    public:
        /**
         * Texture width
         */
        unsigned int Width = 0;

        /**
         * Texture Height
         */
        unsigned int Height = 0;

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
        /**
         * Internal texture (OpenGL)
         */
        std::shared_ptr<OpenGL::GLTexture> InternalTexture = nullptr;
#endif
#endif

        /**
         * Create a null texture
         */
        Texture2D();

        /**
         * Copy constructor.
         */
        Texture2D(const Texture2D &original_) = default;

        /**
         * Create a texture from raw pixel data.
         *
         * @note The pixel data is copied.
         * @param graphicsDevice_ The current graphics device.
         * @param data_ Pixel data.
         * @param width_ Width of the texture.
         * @param height_ Height of the texture.
         * @param format_ Data pixel format.
         * @param mipmapCount_ Number of mipmaps to generate.
         */
        Texture2D(GraphicsDevice *graphicsDevice_, unsigned char *data_, unsigned int width_, unsigned height_, PixelFormat format_ = UNCOMPRESSED_R8G8B8A8, int mipmapCount_ = 1);

        /**
         * Load a texture file.
         *
         * @param graphicsDevice_ The current graphics device.
         * @param path_ Texture file to load.
         */
        Texture2D(GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_);

        /**
         * Create a texture from an image.
         *
         * @param graphicsDevice_ The current graphics device.
         * @param img_ Image to load onto GPU.
         */
        Texture2D(GraphicsDevice *graphicsDevice_, const Image *img_);

        /**
         * Get the number of mipmaps this texture has
         *
         * @return The number of mipmaps generated.
         */
        int GetMipmapCount() const;

        /**
         * Set the texture filter mode.
         *
         * @param filterMode_ The filter mode to set.
         */
        void SetTextureFilter(TextureFilterMode filterMode_) const;

        /**
         * Set the texture wrap mode.
         *
         * @param wrapMode_ The texture wrap mode.
         */
        void SetTextureWrap(TextureWrapMode wrapMode_) const;

        /**
         * Is the texture valid and ready for use.
         *
         * @return Whether or not the texture is valid.
         */
        bool IsValid() const override;

        /**
         * Unload the texture.
         */
        void Unload() override;

        /**
         * Draw the texture.
         *
         * @param pos_ The position to draw at.
         * @param col_ The color modifier.
         * @param scale_ The scale modifier.
         * @param origin_ The origin (for rotation).
         * @param rotation_ Rotation (from origin).
         */
        void Draw(Graphics::Renderer *renderer_, Vector2 pos_, Color col_ = Color::White, float scale_ = 1, Vector2 origin_ = Vector2::Zero, Angle rotation_ = 0);

        /**
         * Draw the texture.
         *
         * @param destRect_ The destination rectangle.
         * @param srcRect_ The source rectangle.
         * @param origin_ The origin (for rotation).
         * @param rotation_ Rotation (from origin).
         */
        void Draw(Graphics::Renderer *renderer_, Rectangle destRect_, Rectangle srcRect_, Color col_ = Color::White, Vector2 origin_ = Vector2::Zero, Angle rotation_ = 0);

        bool operator==(const Texture2D &tex_) const;
        bool operator!=(const Texture2D &tex_) const;

        /*
         * Copy a texture
         */
        Texture2D &operator=(const Texture2D &tex_) = default;
    };
}

#endif //TEXTURE2D_HPP
