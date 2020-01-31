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

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#include "Graphics/OpenGL.hpp"
#endif

#include "Graphics/API/PlatformGraphicsAPI.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/Renderer.hpp"
#include "Console.hpp"

namespace Ngine::Graphics {
    Texture2D::Texture2D() {
        Unload();
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, unsigned char *data_, unsigned int width_,
                         unsigned height_, PixelFormat format_, int mipmapCount_) {
        // Check dimensions
        if (width_ <= 0 || height_ <= 0) {
            Console::Error("Texture2D", "Texture was given invalid dimensions of %u, %u.", width_, height_);
            return;
        }

        // Save API
        m_API = graphicsDevice_->GetAPI();
        m_graphicsDevice = graphicsDevice_;

        // Set fields
        Width = width_;
        Height = height_;
        m_mipmapCount = mipmapCount_;
        m_format = format_;

        // Make our texture
        m_API->CreateTexture(this, data_);
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_) {
        // Create
        Image img(path_);
        *this = Texture2D(graphicsDevice_, &img);
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, const Image *img_) {
        // Create
        *this = Texture2D(graphicsDevice_, img_->PixelData, img_->Width, img_->Height, img_->Format, img_->MipmapCount);
    }

    PixelFormat Texture2D::GetFormat() {
        return m_format;
    }

    int Texture2D::GetMipmapCount() const {
        return m_mipmapCount;
    }

    void Texture2D::SetTextureFilter(const TextureFilterMode filterMode_) const {
        // Bind
        glBindTexture(GL_TEXTURE_2D, ID);
        auto maxAnisotropicLevel = m_graphicsDevice->GetGLMaxAnisotropicLevel();

        switch (filterMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            case FILTER_POINT:
                if (m_mipmapCount > 1) {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                } else {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
                break;
            case FILTER_BILINEAR:
                if (m_mipmapCount > 1) {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                } else {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
                break;
            case FILTER_TRILINEAR:
                if (m_mipmapCount > 1) {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                } else {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
                break;
            case FILTER_ANISOTROPIC_4X:
                if (4 < maxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
                else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropicLevel);
                break;
            case FILTER_ANISOTROPIC_8X:
                if (8 < maxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
                else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropicLevel);
                break;
            case FILTER_ANISOTROPIC_16X:
                if (16 < maxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
                else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropicLevel);
                break;
#endif
            default:
                break;
        }
    }

    void Texture2D::SetTextureWrap(const TextureWrapMode wrapMode_) const {
        // Bind
        glBindTexture(GL_TEXTURE_2D, ID);
        
        switch (wrapMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            case WRAP_REPEAT:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            case WRAP_CLAMP:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            case WRAP_MIRROR_REPEAT:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
            case WRAP_MIRROR_CLAMP:
                // GL_MIRROR_CLAMP_EXT = 0x8742
                if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_TEX_MIRROR_CLAMP)) {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x8742);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x8742);
                } else Console::Warn("GLTexture", "Clamp mirror mode not supported.");
                break;
#endif
        }
    }

    bool Texture2D::IsValid() const {
        return ID != 0 && Width > 0 && Height > 0;
    }

    void Texture2D::Unload() {
        // Delete
        m_API->DeleteTexture(this);

        // Clear fields
        Width = 0;
        Height = 0;
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
                            (srcRect_.Width + srcRect_.X) / (float)Width,
                            (srcRect_.Y) / (float)Height
                    }, col_});
            renderer_->PushVertex({
                    {0, destRect_.Height, 0},
                    {
                            (srcRect_.Width + srcRect_.X) / (float)Width,
                            (srcRect_.Height + srcRect_.Y) / (float)Height
                    }, col_});
            renderer_->PushVertex({
                    {destRect_.Width, destRect_.Height, 0},
                    {
                            (srcRect_.X) / (float)Width,
                            (srcRect_.Height + srcRect_.Y) / (float)Height
                    }, col_});
            renderer_->PushVertex({
                    {destRect_.Width, 0, 0},
                    {
                            (srcRect_.X) / (float)Width,
                            (srcRect_.Y) / (float)Height
                    }, col_});
        } else {
            renderer_->PushVertex({
                    {0, 0, 0},
                    {
                            (srcRect_.X) / (float)Width,
                            (srcRect_.Y) / (float)Height
                    }, col_});
            renderer_->PushVertex({
                    {0, destRect_.Height, 0},
                    {
                            (srcRect_.X) / (float)Width,
                            (srcRect_.Height + srcRect_.Y) / (float)Height
                    }, col_});
            renderer_->PushVertex({
                    {destRect_.Width, destRect_.Height, 0},
                    {
                            (srcRect_.Width + srcRect_.X) / (float)Width,
                            (srcRect_.Height + srcRect_.Y) / (float)Height
                    }, col_});
            renderer_->PushVertex({
                    {destRect_.Width, 0, 0},
                    {
                            (srcRect_.Width + srcRect_.X) / (float)Width,
                            (srcRect_.Y) / (float)Height
                    }, col_});
        }
        renderer_->EndVertices();

        // Pop matrix
        renderer_->PopMatrix();
    }

    bool Texture2D::operator==(const Texture2D &tex_) const {
        return ID == tex_.ID;
    }

    bool Texture2D::operator!=(const Texture2D &tex_) const {
        return ID != tex_.ID;
    }
}
