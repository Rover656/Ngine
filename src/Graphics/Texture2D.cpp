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

#include "Graphics/Texture2D.hpp"

#include "Graphics/API/PlatformGraphicsAPI.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/Renderer.hpp"
#include "Console.hpp"

namespace Ngine::Graphics {
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
        m_API = graphicsDevice_->GetAPI();

        // Set fields
        m_mipmapCount = mipmapCount_;
        m_format = format_;

        // Make our texture
        m_API->CreateTexture(this, data_);
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, const Image *img_)
            : Texture2D(graphicsDevice_, img_->PixelData, img_->Width, img_->Height, img_->Format, img_->MipmapCount) {}

    Texture2D *Texture2D::LoadTexture(GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_) {
        Image img(path_);
        return new Texture2D(graphicsDevice_, &img);
    }

    Texture2D::~Texture2D() {
        Unload();
    }

    PixelFormat Texture2D::GetFormat() {
        return m_format;
    }

    int Texture2D::GetMipmapCount() const {
        return m_mipmapCount;
    }

    void Texture2D::SetTextureFilter(TextureFilterMode filterMode_) {
        // Set filter
        m_API->SetTextureFilterMode(this, filterMode_);
    }

    void Texture2D::SetTextureWrap(TextureWrapMode wrapMode_) {
        // Set wrap
        m_API->SetTextureWrapMode(this, wrapMode_);
    }

    bool Texture2D::IsValid() const {
        return Width > 0 && Height > 0 && m_API->IsTextureValid(this);
    }

    void Texture2D::Unload() {
        // Delete
        m_API->DeleteTexture(this);
    }

    void Texture2D::Draw(Graphics::Renderer *renderer_, Vector2 pos_, Color col_, float scale_, Vector2 origin_,
                         Angle rotation_) {
        Draw(renderer_,
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
    Texture2D::Draw(Graphics::Renderer *renderer_, Rectangle destRect_, Rectangle srcRect_, Color col_, Vector2 origin_,
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
        renderer_->PushMatrix();
        renderer_->Translate({destRect_.X, destRect_.Y, 0});
        renderer_->Rotate(rotation_, {0, 0, 1});
        renderer_->Translate({-origin_.X, -origin_.Y, 0});

        // Push vertices
        renderer_->SetTexture(this);
        renderer_->BeginVertices(PrimitiveType::Quads);
        if (flipX) {
            renderer_->PushVertex({
                                          {0, 0, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->PushVertex({
                                          {0, destRect_.Height, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->PushVertex({
                                          {destRect_.Width, destRect_.Height, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->PushVertex({
                                          {destRect_.Width, 0, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
        } else {
            renderer_->PushVertex({
                                          {0, 0, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->PushVertex({
                                          {0, destRect_.Height, 0},
                                          {
                                                  (srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->PushVertex({
                                          {destRect_.Width, destRect_.Height, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Height + srcRect_.Y) / (float) Height
                                          }, col_});
            renderer_->PushVertex({
                                          {destRect_.Width, 0, 0},
                                          {
                                                  (srcRect_.Width + srcRect_.X) / (float) Width,
                                                  (srcRect_.Y) / (float) Height
                                          }, col_});
        }
        renderer_->EndVertices();

        // Pop matrix
        renderer_->PopMatrix();
    }

    bool Texture2D::operator==(const Texture2D &tex_) const {
        return m_API->CompareTextures(this, &tex_);
    }

    bool Texture2D::operator!=(const Texture2D &tex_) const {
        return !m_API->CompareTextures(this, &tex_);
    }
}
