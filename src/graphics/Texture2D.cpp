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

#include "graphics/Texture2D.hpp"

#include "graphics/API/PlatformGraphicsAPI.hpp"
#include "graphics/Renderer.hpp"
#include "Console.hpp"

namespace ngine::graphics {
    Texture2D::Texture2D(GraphicsDevice *graphicsDevice, unsigned char *data, unsigned int width,
                         unsigned height, PixelFormat format, int mipmapCount)
            : Width(width), Height(height) {
        // Check dimensions
        if (width <= 0 || height <= 0) {
            Console::Error("Texture2D", "Texture was given invalid dimensions of %u, %u.", width, height);
            return;
        }

        // Check mipmap count
        if (mipmapCount <= 0)
            Console::Fail("Texture2D", "Texture cannot have no mipmaps.");

        // Save API
        m_API = graphicsDevice->getAPI();

        // Set fields
        m_mipmapCount = mipmapCount;
        m_format = format;

        // Make our texture
        m_API->createTexture(this, data);
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice, const Image *img)
            : Texture2D(graphicsDevice, img->PixelData, img->Width, img->Height, img->Format, img->MipmapCount) {}

    Texture2D *Texture2D::LoadTexture(GraphicsDevice *graphicsDevice, const filesystem::Path &path) {
        Image img(path);
        return new Texture2D(graphicsDevice, &img);
    }

    Texture2D::~Texture2D() {
        free();
    }

    PixelFormat Texture2D::getFormat() {
        return m_format;
    }

    int Texture2D::getMipmapCount() const {
        return m_mipmapCount;
    }

    void Texture2D::setTextureFilter(TextureFilterMode filterMode) {
        // Set filter
        m_API->setTextureFilterMode(this, filterMode);
    }

    void Texture2D::setTextureWrap(TextureWrapMode wrapMode) {
        // Set wrap
        m_API->setTextureWrapMode(this, wrapMode);
    }

    bool Texture2D::isValid() const {
        return Width > 0 && Height > 0 && m_API->isTextureValid(this);
    }

    void Texture2D::free() {
        // Delete
        m_API->deleteTexture(this);
    }

    void Texture2D::draw(graphics::Renderer *renderer, Vector2 pos, Color col, float scale, Vector2 origin,
                         Angle rotation) {
        draw(renderer,
             {
                     pos.X,
                     pos.Y,
                     (float) Width * scale,
                     (float) Height * scale
             },
             {0,
              0,
              (float) Width,
              (float) Height
             },
             col,
             origin,
             rotation);
    }

    void
    Texture2D::draw(graphics::Renderer *renderer, Rectangle destRect, Rectangle srcRect, Color col_, Vector2 origin,
                    Angle rotation) {
        // Get origin in pixel coords
        Vector2 pixelOrigin = {origin.X * destRect.Width, origin.Y * destRect.Height};

        // Push transformation matrix
        renderer->pushMatrix();

        // Move to destination
        renderer->translate({destRect.X, destRect.Y, 0});

        // Apply rotation (this fixes issues with different Y stuffs)
        if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
            renderer->rotate(rotation, {0, 0, 1});
        else renderer->rotate(rotation, {0, 0, -1});

        // Fix origin
        renderer->translate({-pixelOrigin.X, -pixelOrigin.Y, 0});

        // Push vertices
        renderer->setTexture(this);
        renderer->beginVertices(PrimitiveType::Quads);

        // Get correct dimensions
        float x1,y1,x2,y2,srcX1,srcY1,srcX2,srcY2;

        // Set common values
        x1 = 0;
        x2 = destRect.Width;
        srcX1 = srcRect.X / Width;
        srcY1 = srcRect.Y / Height;
        srcX2 = (srcRect.X + srcRect.Width) / Width;
        srcY2 = (srcRect.Y + srcRect.Height) / Height;

        if (renderer->getCoordinateSystem() != CoordinateSystem::GUI) {
            // Start in top-left
            y1 = destRect.Height;
            y2 = 0;
        } else {
            y1 = 0;
            y2 = destRect.Height;
        }

        // Push vertices
        renderer->pushVertex({{x1, y1, 0}, {srcX1, srcY1}, col_});
        renderer->pushVertex({{x2, y1, 0}, {srcX2, srcY1}, col_});
        renderer->pushVertex({{x2, y2, 0}, {srcX2, srcY2}, col_});
        renderer->pushVertex({{x1, y2, 0}, {srcX1, srcY2}, col_});

        // Finish
        renderer->endVertices();

        // Pop matrix
        renderer->popMatrix();
    }

    bool Texture2D::operator==(const Texture2D &tex) const {
        return m_API->compareTextures(this, &tex);
    }

    bool Texture2D::operator!=(const Texture2D &tex) const {
        return !m_API->compareTextures(this, &tex);
    }
}
