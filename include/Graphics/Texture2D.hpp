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

#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include "../Config.hpp"

#include "../Filesystem/Filesystem.hpp"
#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "../Resource.hpp"
#include "Color.hpp"
#include "Image.hpp"

namespace Ngine::Graphics {
    class GraphicsDevice;
    class Renderer;
    class RenderTarget;

    class NewRenderer;

    namespace API {
        class PlatformGraphicsAPI;
    }

    /**
     * Texture filter mode.
     */
    enum class TextureFilterMode {
        /**
         * No filter, just approximation.
         */
        Point = 0,

        /**
         * Linear filtering.
         */
        Bilinear,

        /**
         * Trilinear filtering (with mipmaps).
         */
        Trilinear,

        /**
         * Anisotropic filtering 4x.
         */
        Anisotropic_4X,

        /**
         * Anisotropic filtering 8x.
         */
        Anisotropic_8X,

        /**
         * Anisotropic filtering 16x.
         */
        Anisotropic_16X
    };

    /**
     * Texture wrap mode.
     */
    enum class TextureWrapMode {
        /**
         * Repeats texture
         */
        Repeat = 0,

        /**
         * Clamps texture to edge pixel.
         */
        Clamp,

        /**
         * Mirrors and repeats the texture.
         */
        MirrorRepeat
    };

    /**
     * A 2D Texture stored in the GPU memory.
     */
    class NEAPI Texture2D final : public IResource {
        /**
         * The number of mipmaps.
         */
        unsigned int m_mipmapCount = 0;

        /**
         * The pixel format.
         */
        PixelFormat m_format = UNCOMPRESSED_R8G8B8A8;

        /**
         * Graphics API.
         */
        API::PlatformGraphicsAPI *m_API = nullptr;
    public:
        union {
            /**
             * GPU ID (OpenGL)
             */
            unsigned int ID = 0;

            /**
             * GPU Pointer.
             */
            void *Ptr;
        };

        /**
         * Texture width
         */
        const unsigned int Width = 0;

        /**
         * Texture Height
         */
        const unsigned int Height = 0;

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
         * Create a texture from an image.
         *
         * @param graphicsDevice_ The current graphics device.
         * @param img_ Image to load onto GPU.
         */
        Texture2D(GraphicsDevice *graphicsDevice_, const Image *img_);
        ~Texture2D();

        /**
         * Load a texture from a file.
         *
         * @param graphicsDevice_ The current graphics device.
         * @param path_ Texture file to load.
         */
        static Texture2D *LoadTexture(GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_);

        /**
         * Get the texture pixel format.
         */
        PixelFormat GetFormat();

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
        void SetTextureFilter(TextureFilterMode filterMode_);

        /**
         * Set the texture wrap mode.
         *
         * @param wrapMode_ The texture wrap mode.
         */
        void SetTextureWrap(TextureWrapMode wrapMode_);

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
