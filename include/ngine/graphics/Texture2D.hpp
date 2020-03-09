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

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "../Resource.hpp"
#include "../filesystem/Filesystem.hpp"
#include "Color.hpp"
#include "Image.hpp"

namespace ngine::graphics {
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
        Texture2D(const Texture2D &original) = default;

        /**
         * Create a texture from raw pixel data.
         *
         * @note The pixel data is copied.
         * @param graphicsDevice The current graphics device.
         * @param data Pixel data.
         * @param width Width of the texture.
         * @param height Height of the texture.
         * @param format Data pixel format.
         * @param mipmapCount Number of mipmaps to generate.
         */
        Texture2D(GraphicsDevice *graphicsDevice, unsigned char *data,
                  unsigned int width, unsigned int height,
                  PixelFormat format = UNCOMPRESSED_R8G8B8A8,
                  int mipmapCount = 1);

        /**
         * Create a texture from an image.
         *
         * @param graphicsDevice The current graphics device.
         * @param img Image to load onto GPU.
         */
        Texture2D(GraphicsDevice *graphicsDevice, const Image *img);

        ~Texture2D();

        /**
         * Load a texture from a file.
         *
         * @param graphicsDevice The current graphics device.
         * @param path Texture file to load.
         */
        static Texture2D *loadTexture(GraphicsDevice *graphicsDevice,
                                      const filesystem::Path &path);

        /**
         * Get the texture pixel format.
         */
        PixelFormat getFormat();

        /**
         * Get the number of mipmaps this texture has
         *
         * @return The number of mipmaps generated.
         */
        int getMipmapCount() const;

        /**
         * Set the texture filter mode.
         *
         * @param filterMode The filter mode to set.
         */
        void setTextureFilter(TextureFilterMode filterMode);

        /**
         * Set the texture wrap mode.
         *
         * @param wrapMode The texture wrap mode.
         */
        void setTextureWrap(TextureWrapMode wrapMode);

        /**
         * Is the texture valid and ready for use.
         *
         * @return Whether or not the texture is valid.
         */
        bool isValid() const override;

        /**
         * Free the texture.
         */
        void free() override;

        /**
         * Draw the texture.
         *
         * @param pos The position to draw at (top-left).
         * @param col The color modifier.
         * @param scale The scale modifier.
         * @param origin The origin (in percentages (0 -> 1)).
         * @param rotation Rotation (from origin).
         */
        void draw(graphics::Renderer *renderer, Vector2 pos,
                  Color col = Color::White, float scale = 1,
                  Vector2 origin = {0.5f, 0.5f}, Angle rotation = 0);

        /**
         * Draw the texture.
         *
         * @param destRect The destination rectangle (Position refers to top
         * left).
         * @param srcRect The source rectangle.
         * @param origin The origin (in percentages (0 -> 1)).
         * @param rotation Rotation (from origin).
         */
        void draw(graphics::Renderer *renderer, Rectangle destRect,
                  Rectangle srcRect, Color col = Color::White,
                  Vector2 origin = {0.5f, 0.5f}, Angle rotation = 0);

        bool operator==(const Texture2D &tex) const;

        bool operator!=(const Texture2D &tex) const;

        /*
         * Copy a texture
         */
        Texture2D &operator=(const Texture2D &tex) = default;
    };
} // namespace ngine::graphics

#endif // TEXTURE2D_HPP
