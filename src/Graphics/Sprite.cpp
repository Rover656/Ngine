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

#include "Sprite.hpp"

#include <cmath>

#include "Color.hpp"
#include "../Rectangle.hpp"
#include "Renderer.hpp"

namespace NerdThings::Ngine::Graphics {
    Sprite::Sprite(Texture2D *texture_) {
        // Add texture
        m_textures.push_back(texture_);

        // Set draw dimensions
        DrawHeight = texture_->Height;
        DrawWidth = texture_->Width;

        // Mark as spritesheet
        m_spriteSheet = true;
    }

    Sprite::Sprite(Texture2D *texture_, int frameWidth_, int frameHeight_, int drawWidth_, int drawHeight_,
                   float imageSpeed_, int startingFrame)
            : DrawHeight(drawHeight_), DrawWidth(drawWidth_), FrameWidth(frameWidth_), FrameHeight(frameHeight_),
              ImageSpeed(imageSpeed_) {
        // Add texture
        m_textures.push_back(texture_);

        // Set frame
        CurrentFrame = startingFrame;

        // Mark as spritesheet
        m_spriteSheet = true;
    }

    Sprite::Sprite(const std::vector<Texture2D *> &textures_, float imageSpeed_, int startingFrame_) {
        // Set textures
        for (auto tex : textures_) {
            m_textures.push_back(tex);
        }

        // Set frame
        CurrentFrame = startingFrame_;

        // Mark as not spritesheet
        m_spriteSheet = false;
    }

    void Sprite::Draw(Vector2 position_, float scale_, float rotation_, Vector2 origin_) {
        if (m_textures.empty())
            return;

        GetCurrentTexture()->Draw(
                {
                        position_,
                        static_cast<float>(DrawWidth * scale_),
                        static_cast<float>(DrawHeight * scale_)
                },
                GetSourceRectangle(),
                Color::White,
                origin_,
                rotation_);
    }

    void Sprite::Update() {
        if (IsAnimated()) {
            // Increment timer
            m_animationTimer++;

            while (m_animationTimer > 0 && fmod(m_animationTimer, ImageSpeed) == 0) {
                // Reset timer
                m_animationTimer = 0;

                // Increase frame
                CurrentFrame++;

                // Reset if out of range
                if (m_spriteSheet) {
                    auto count =
                            (GetCurrentTexture()->Width / FrameWidth) * (GetCurrentTexture()->Height / FrameHeight);

                    if (CurrentFrame > count - 1)
                        CurrentFrame = 0;
                } else {
                    if (CurrentFrame > m_textures.size() - 1)
                        CurrentFrame = 0;
                }
            }
        }
    }

    bool Sprite::IsAnimated() {
        if (m_spriteSheet) {
            if (GetCurrentTexture()->IsValid())
                return FrameHeight != GetCurrentTexture()->Height || FrameWidth != GetCurrentTexture()->Width;
            else
                return false;
        } else {
            return m_textures.size() > 1;
        }
    }

    int Sprite::FrameX() {
        if (!m_spriteSheet || m_textures.empty())
            return 0;

        auto x = 0;
        for (auto i = 0; i < CurrentFrame; i++) {
            x += FrameWidth;
            if (x >= GetCurrentTexture()->Width)
                x = 0;
        }

        return x;
    }

    int Sprite::FrameY() {
        if (!m_spriteSheet || m_textures.empty())
            return 0;

        auto x = 0;
        auto y = 0;
        for (auto i = 0; i < CurrentFrame; i++) {
            x += FrameWidth;
            if (x >= GetCurrentTexture()->Width) {
                x = 0;
                y += FrameHeight;
            }
        }

        return y;
    }

    Texture2D *Sprite::GetCurrentTexture() {
        if (m_textures.empty())
            return nullptr;

        if (m_spriteSheet) {
            return m_textures[0];
        }

        return m_textures[CurrentFrame];
    }

    void Sprite::SetTexture(Texture2D *texture_) {
        m_textures.clear();
        m_textures.push_back(texture_);
    }

    void Sprite::SetTextures(const std::vector<Texture2D *> &textures_) {
        m_textures.clear();

        for (auto t : textures_) {
            // Look for existing shared pointer first.
            m_textures.push_back(t);
        }
    }

    Rectangle Sprite::GetSourceRectangle() {
        if (m_spriteSheet)
            return {
                    static_cast<float>(FrameX()),
                    static_cast<float>(FrameY()),
                    static_cast<float>(FrameWidth),
                    static_cast<float>(FrameHeight)
            };
        else
            return {
                    0,
                    0,
                    static_cast<float>(GetCurrentTexture()->Width),
                    static_cast<float>(GetCurrentTexture()->Height)
            };
    }

    bool Sprite::operator==(const Sprite &b) {
        return m_textures == b.m_textures && DrawHeight == b.DrawHeight && DrawWidth == b.DrawWidth &&
               FrameHeight == b.FrameHeight && FrameWidth == b.FrameWidth && ImageSpeed == b.ImageSpeed;
    }

    bool Sprite::operator!=(const Sprite &b) {
        return m_textures != b.m_textures || DrawHeight != b.DrawHeight || DrawWidth != b.DrawWidth ||
               FrameHeight != b.FrameHeight || FrameWidth != b.FrameWidth || ImageSpeed != b.ImageSpeed;
    }
}
