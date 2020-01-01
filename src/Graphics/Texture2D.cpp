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

#include "Texture2D.hpp"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
#include "OpenGL.hpp"
#endif
#endif

#include "../Logger.hpp"
#include "GraphicsDevice.hpp"
#include "Renderer.hpp"

namespace NerdThings::Ngine::Graphics {
#ifdef USE_EXPERIMENTAL_RENDERER
    void Texture2D::_createTexture(GraphicsDevice *graphicsDevice_, unsigned char *data_) {
        // Unbind any bound textures
        glBindTexture(GL_TEXTURE_2D, 0);

        // Check format support
        if ((!graphicsDevice_->GetGLSupportFlag(GraphicsDevice::GL_COMP_DXT))
            && ((m_format == COMPRESSED_DXT1_RGB)
                || (m_format == COMPRESSED_DXT1_RGBA)
                || (m_format == COMPRESSED_DXT3_RGBA)
                || (m_format == COMPRESSED_DXT5_RGBA))) {
            throw std::runtime_error("DXT compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(GraphicsDevice::GL_COMP_ETC1))
            && (m_format == COMPRESSED_ETC1_RGB)) {
            throw std::runtime_error("ETC1 compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(GraphicsDevice::GL_COMP_ETC2)) &&
            ((m_format == COMPRESSED_ETC2_RGB) || (m_format == COMPRESSED_ETC2_EAC_RGBA))) {
            throw std::runtime_error("ETC2 compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(GraphicsDevice::GL_COMP_PVRT)) &&
            ((m_format == COMPRESSED_PVRT_RGB) || (m_format == COMPRESSED_PVRT_RGBA))) {
            throw std::runtime_error("PVRT compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(GraphicsDevice::GL_COMP_ASTC)) &&
            ((m_format == COMPRESSED_ASTC_4x4_RGBA) || (m_format == COMPRESSED_ASTC_8x8_RGBA))) {
            throw std::runtime_error("ASTC compressed texture format not supported");
        }

        // Create texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &m_ID);

        // Bind
        glBindTexture(GL_TEXTURE_2D, m_ID);

        // Generate mipmaps
        int mipWidth = Width;
        int mipHeight = Height;
        int mipOffset = 0;

        unsigned int glInternalFormat, glFormat, glType;
        graphicsDevice_->GetGLTextureFormats(m_format, &glInternalFormat, &glFormat, &glType);

        for (int i = 0; i < m_mipmapCount; i++) {
            unsigned int mipSize = _calculatePixelDataSize();

            if (glInternalFormat != -1) {
                if (m_format < COMPRESSED_DXT1_RGB) glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, (unsigned char *)data_ + mipOffset);
                else glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, mipSize, (unsigned char *)data_ + mipOffset);

#if defined(GRAPHICS_OPENGL33)
                if (m_format == UNCOMPRESSED_GRAYSCALE)
                {
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }
                else if (m_format == UNCOMPRESSED_GRAY_ALPHA)
                {
#if defined(GRAPHICS_OPENGL21)
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ALPHA };
#elif defined(GRAPHICS_OPENGL33)
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
#endif
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }
#endif
            }

            mipWidth /= 2;
            mipHeight /= 2;
            mipOffset += mipSize;

            // Security check for NPOT textures
            if (mipWidth < 1) mipWidth = 1;
            if (mipHeight < 1) mipHeight = 1;
        }

        // TEMP: Needs proper implementation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    int Texture2D::_calculatePixelDataSize() {
        auto bpp = 0;

        switch (m_format)
        {
            case UNCOMPRESSED_GRAYSCALE: bpp = 8; break;
            case UNCOMPRESSED_GRAY_ALPHA:
            case UNCOMPRESSED_R5G6B5:
            case UNCOMPRESSED_R5G5B5A1:
            case UNCOMPRESSED_R4G4B4A4: bpp = 16; break;
            case UNCOMPRESSED_R8G8B8A8: bpp = 32; break;
            case UNCOMPRESSED_R8G8B8: bpp = 24; break;
            case UNCOMPRESSED_R32: bpp = 32; break;
            case UNCOMPRESSED_R32G32B32: bpp = 32*3; break;
            case UNCOMPRESSED_R32G32B32A32: bpp = 32*4; break;
            case COMPRESSED_DXT1_RGB:
            case COMPRESSED_DXT1_RGBA:
            case COMPRESSED_ETC1_RGB:
            case COMPRESSED_ETC2_RGB:
            case COMPRESSED_PVRT_RGB:
            case COMPRESSED_PVRT_RGBA: bpp = 4; break;
            case COMPRESSED_DXT3_RGBA:
            case COMPRESSED_DXT5_RGBA:
            case COMPRESSED_ETC2_EAC_RGBA:
            case COMPRESSED_ASTC_4x4_RGBA: bpp = 8; break;
            case COMPRESSED_ASTC_8x8_RGBA: bpp = 2; break;
            default: break;
        }

        auto dataSize = Width*Height*bpp/8;  // Total data size in bytes

        // Most compressed formats works on 4x4 blocks,
        // if texture is smaller, minimum dataSize is 8 or 16
        if ((Width < 4) && (Height < 4))
        {
            if ((m_format >= COMPRESSED_DXT1_RGB) && (m_format < COMPRESSED_DXT3_RGBA)) dataSize = 8;
            else if ((m_format >= COMPRESSED_DXT3_RGBA) && (m_format < COMPRESSED_ASTC_8x8_RGBA)) dataSize = 16;
        }

        return dataSize;
    }
#endif

    Texture2D::Texture2D() {
        Unload();
    }

    Texture2D::Texture2D(GraphicsDevice *graphicsDevice_, unsigned char *data_, unsigned int width_,
                         unsigned height_, PixelFormat format_, int mipmapCount_) {
        // Check dimensions
        if (width_ <= 0 || height_ <= 0) {
            Logger::Error("Texture2D", "Texture was given invalid dimensions of %u, %u.", width_, height_);
            return;
        }

        Width = width_;
        Height = height_;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
        // Set fields
        m_mipmapCount = mipmapCount_;
        m_format = format_;

        // Make our texture
        _createTexture(graphicsDevice_, data_);
#else
        // Get format
        OpenGL::GLPixelFormat frmt = OpenGL::UNCOMPRESSED_GRAYSCALE;
        if (format_ == UNCOMPRESSED_GRAY_ALPHA) frmt = OpenGL::UNCOMPRESSED_GRAY_ALPHA;
        else if (format_ == UNCOMPRESSED_R8G8B8) frmt = OpenGL::UNCOMPRESSED_R8G8B8;
        else if (format_ == UNCOMPRESSED_R8G8B8A8) frmt = OpenGL::UNCOMPRESSED_R8G8B8A8;
        else throw std::runtime_error("Incompatible format.");

        InternalTexture = std::make_shared<OpenGL::GLTexture>(width_, height_, data_, mipmapCount_, frmt);
#endif
#endif
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

    int Texture2D::GetMipmapCount() const {
#ifdef USE_EXPERIMENTAL_RENDERER
        return m_mipmapCount;
#else
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalTexture->MipmapCount;
#endif
#endif
        return 0;
    }

    void Texture2D::SetTextureFilter(const TextureFilterMode filterMode_) const {
        switch (filterMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
            case FILTER_POINT:
                if (InternalTexture->MipmapCount > 1) {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_MIP_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                } else {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                }
                break;
            case FILTER_BILINEAR:
                if (InternalTexture->MipmapCount > 1) {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_LINEAR_MIP_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                } else {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                }
                break;
            case FILTER_TRILINEAR:
                if (InternalTexture->MipmapCount > 1) {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_MIP_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                } else {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                }
                break;
            case FILTER_ANISOTROPIC_4X:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_ANISOTROPIC_FILTER, 4);
                break;
            case FILTER_ANISOTROPIC_8X:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_ANISOTROPIC_FILTER, 8);
                break;
            case FILTER_ANISOTROPIC_16X:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_ANISOTROPIC_FILTER, 16);
                break;
#endif
#endif
            default:
                break;
        }
    }

    void Texture2D::SetTextureWrap(const TextureWrapMode wrapMode_) const {
        switch (wrapMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
            case WRAP_REPEAT:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_REPEAT);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_REPEAT);
                break;
            case WRAP_CLAMP:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_CLAMP);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_CLAMP);
                break;
            case WRAP_MIRROR_REPEAT:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_MIRROR_REPEAT);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_MIRROR_REPEAT);
                break;
            case WRAP_MIRROR_CLAMP:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_MIRROR_CLAMP);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_MIRROR_CLAMP);
                break;
#endif
#endif
        }
    }

    bool Texture2D::IsValid() const {
#ifdef USE_EXPERIMENTAL_RENDERER
        return m_ID != 0 && Width > 0 && Height > 0;
#else
        if (InternalTexture != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            if (InternalTexture->ID > 0)
                return true;
#endif
#endif
        return false;
    }

    void Texture2D::Unload() {
        // Delete texture data
        Width = 0;
        Height = 0;

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
        glDeleteTextures(1, &m_ID);
        m_ID = 0;
#else
        InternalTexture = nullptr;
#endif
#endif
    }

    void Texture2D::Draw(Vector2 pos_, Color col_, float scale_, Vector2 origin_, Angle rotation_) {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("Not implemented.");
#else
        Renderer::DrawTexture(this, pos_, col_, scale_, origin_, rotation_.GetDegrees());
#endif
    }

    void Texture2D::Draw(Rectangle destRect_, Rectangle srcRect_, Color col_, Vector2 origin_, Angle rotation_) {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("Not implemented.");
#else
        Renderer::DrawTexture(this, destRect_, srcRect_, col_, origin_, rotation_.GetDegrees());
#endif
    }

    bool Texture2D::operator==(const Texture2D &tex_) const {
#ifdef USE_EXPERIMENTAL_RENDERER
        return m_ID == tex_.m_ID;
#else
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalTexture->ID == tex_.InternalTexture->ID;
#endif
#endif
        return false;
    }

    bool Texture2D::operator!=(const Texture2D &tex_) const {
#ifdef USE_EXPERIMENTAL_RENDERER
        return m_ID != tex_.m_ID;
#else
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalTexture->ID != tex_.InternalTexture->ID;
#endif
#endif
        return true;
    }
}
