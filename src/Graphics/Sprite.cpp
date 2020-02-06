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

#include "Graphics/Sprite.hpp"

#include <cmath>

#include "Graphics/Color.hpp"
#include "Graphics/Renderer.hpp"
#include "Rectangle.hpp"

namespace ngine::graphics {
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
              FrameTime(imageSpeed_) {
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

    void Sprite::draw(graphics::Renderer *renderer_, Vector2 position_, float scale_, float rotation_, Vector2 origin_) {
        if (m_textures.empty())
            return;

        getCurrentTexture()->draw(
                renderer_,
                {
                        position_,
                        static_cast<float>(DrawWidth * scale_),
                        static_cast<float>(DrawHeight * scale_)
                },
                getSourceRectangle(),
                Color::White,
                origin_,
                rotation_);
    }

    void Sprite::update() {
        if (isAnimated()) {
            // Increment timer
            m_animationTimer++;

            while (m_animationTimer > 0 && fmod(m_animationTimer, FrameTime) == 0) {
                // Reset timer
                m_animationTimer = 0;

                // Increase frame
                CurrentFrame++;

                // Reset if out of range
                if (m_spriteSheet) {
                    auto count =
                            (getCurrentTexture()->Width / FrameWidth) * (getCurrentTexture()->Height / FrameHeight);

                    if (CurrentFrame > count - 1)
                        CurrentFrame = 0;
                } else {
                    if (CurrentFrame > m_textures.size() - 1)
                        CurrentFrame = 0;
                }
            }
        }
    }

    bool Sprite::isAnimated() {
        if (m_spriteSheet) {
            if (getCurrentTexture()->isValid())
                return FrameHeight != getCurrentTexture()->Height || FrameWidth != getCurrentTexture()->Width;
            else
                return false;
        } else {
            return m_textures.size() > 1;
        }
    }

    int Sprite::getFrameX() {
        if (!m_spriteSheet || m_textures.empty())
            return 0;

        auto x = 0;
        for (auto i = 0; i < CurrentFrame; i++) {
            x += FrameWidth;
            if (x >= getCurrentTexture()->Width)
                x = 0;
        }

        return x;
    }

    int Sprite::getFrameY() {
        if (!m_spriteSheet || m_textures.empty())
            return 0;

        auto x = 0;
        auto y = 0;
        for (auto i = 0; i < CurrentFrame; i++) {
            x += FrameWidth;
            if (x >= getCurrentTexture()->Width) {
                x = 0;
                y += FrameHeight;
            }
        }

        return y;
    }

    Texture2D *Sprite::getCurrentTexture() {
        if (m_textures.empty())
            return nullptr;

        if (m_spriteSheet) {
            return m_textures[0];
        }

        return m_textures[CurrentFrame];
    }

    void Sprite::setTexture(Texture2D *texture_) {
        m_textures.clear();
        m_textures.push_back(texture_);
    }

    void Sprite::setTextures(const std::vector<Texture2D *> &textures_) {
        m_textures.clear();

        for (auto t : textures_) {
            // Look for existing shared pointer first.
            m_textures.push_back(t);
        }
    }

    Rectangle Sprite::getSourceRectangle() {
        if (m_spriteSheet)
            return {
                    static_cast<float>(getFrameX()),
                    static_cast<float>(getFrameY()),
                    static_cast<float>(FrameWidth),
                    static_cast<float>(FrameHeight)
            };
        else
            return {
                    0,
                    0,
                    static_cast<float>(getCurrentTexture()->Width),
                    static_cast<float>(getCurrentTexture()->Height)
            };
    }

    bool Sprite::operator==(const Sprite &b) {
        return m_textures == b.m_textures && DrawHeight == b.DrawHeight && DrawWidth == b.DrawWidth &&
               FrameHeight == b.FrameHeight && FrameWidth == b.FrameWidth && FrameTime == b.FrameTime;
    }

    bool Sprite::operator!=(const Sprite &b) {
        return m_textures != b.m_textures || DrawHeight != b.DrawHeight || DrawWidth != b.DrawWidth ||
               FrameHeight != b.FrameHeight || FrameWidth != b.FrameWidth || FrameTime != b.FrameTime;
    }
}
