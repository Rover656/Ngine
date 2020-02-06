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
    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, unsigned char *data_, unsigned int width_,
                         unsigned height_, PixelFormat format_, int mipmapCount_)
            : Width(width_), Height(height_) {
        // Check dimensions
        if (width_ <= 0 || height_ <= 0) {
            Console::Error("Texture2D", "Texture was given invalid dimensions of %u, %u.", width_, height_);
            return;
        }

        // Check mipmap count
        if (mipmapCount_ <= 0)
            Console::Fail("Texture2D", "Texture cannot have no mipmaps.");

        // Save API
        m_API = graphicsDevice_->getAPI();

        // Set fields
        m_mipmapCount = mipmapCount_;
        m_format = format_;

        // Make our texture
        m_API->createTexture(this, data_);
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, const Image *img_)
            : Texture2D(graphicsDevice_, img_->PixelData, img_->Width, img_->Height, img_->Format, img_->MipmapCount) {}

    Texture2D *Texture2D::LoadTexture(GraphicsDevice *graphicsDevice_, const filesystem::Path &path_) {
        Image img(path_);
        return new Texture2D(graphicsDevice_, &img);
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

    void Texture2D::setTextureFilter(TextureFilterMode filterMode_) {
        // Set filter
        m_API->setTextureFilterMode(this, filterMode_);
    }

    void Texture2D::setTextureWrap(TextureWrapMode wrapMode_) {
        // Set wrap
        m_API->setTextureWrapMode(this, wrapMode_);
    }

    bool Texture2D::isValid() const {
        return Width > 0 && Height > 0 && m_API->isTextureValid(this);
    }

    void Texture2D::free() {
        // Delete
        m_API->deleteTexture(this);
    }

    void Texture2D::draw(graphics::Renderer *renderer_, Vector2 pos_, Color col_, float scale_, Vector2 origin_,
                         Angle rotation_) {
        draw(renderer_,
             {
                     0,
                     0,
                     (float) Width * scale_,
                     (float) Height * scale_
             },
             {0,
              0,
              (float) Width,
              (float) Height
             },
             col_,
             origin_,
             rotation_);
    }

    void
    Texture2D::draw(graphics::Renderer *renderer_, Rectangle destRect_, Rectangle srcRect_, Color col_, Vector2 origin_,
                    Angle rotation_) {
        // Fix parameters
        bool flipX = false;
        if (srcRect_.Width < 0) {
            srcRect_.Width *= -1;
            flipX = true;
        }

        if (srcRect_.Height < 0) {
            srcRect_.Y -= srcRect_.Height;
        }

        // Push transformation matrix
        renderer_->pushMatrix();
        renderer_->translate({destRect_.X, destRect_.Y, 0});
        renderer_->rotate(rotation_, {0, 0, 1});
        renderer_->translate({-origin_.X, -origin_.Y, 0});

        // Push vertices
        renderer_->setTexture(this);
        renderer_->beginVertices(PrimitiveType::Quads);
        if (flipX) {
            renderer_->pushVertex({
                                          {0, 0, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->pushVertex({
                                          {0, destRect_.Height, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->pushVertex({
                                          {destRect_.Width, destRect_.Height, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->pushVertex({
                                          {destRect_.Width, 0, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
        } else {
            renderer_->pushVertex({
                                          {0, 0, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->pushVertex({
                                          {0, destRect_.Height, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->pushVertex({
                                          {destRect_.Width, destRect_.Height, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->pushVertex({
                                          {destRect_.Width, 0, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
        }
        renderer_->endVertices();

        // Pop matrix
        renderer_->popMatrix();
    }

    bool Texture2D::operator==(const Texture2D &tex_) const {
        return m_API->compareTextures(this, &tex_);
    }

    bool Texture2D::operator!=(const Texture2D &tex_) const {
        return !m_API->compareTextures(this, &tex_);
    }
}
