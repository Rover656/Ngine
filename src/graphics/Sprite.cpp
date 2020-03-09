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

#include "ngine/graphics/Sprite.hpp"

#include <cmath>

#include "ngine/Rectangle.hpp"
#include "ngine/graphics/Color.hpp"
#include "ngine/graphics/Renderer.hpp"

namespace ngine::graphics {
    Sprite::Sprite(Texture2D *texture) {
        // Add texture
        m_textures.push_back(texture);

        // Set draw dimensions
        DrawHeight = texture->Height;
        DrawWidth = texture->Width;

        // Mark as spritesheet
        m_spriteSheet = true;
    }

    Sprite::Sprite(Texture2D *texture, int frameWidth, int frameHeight,
                   int drawWidth, int drawHeight, float imageSpeed,
                   int startingFrame)
        : DrawHeight(drawHeight), DrawWidth(drawWidth), FrameWidth(frameWidth),
          FrameHeight(frameHeight), FrameTime(imageSpeed) {
        // Add texture
        m_textures.push_back(texture);

        // Set frame
        CurrentFrame = startingFrame;

        // Mark as spritesheet
        m_spriteSheet = true;
    }

    Sprite::Sprite(const std::vector<Texture2D *> &textures, float imageSpeed,
                   int startingFrame) {
        // Set textures
        for (auto tex : textures) {
            m_textures.push_back(tex);
        }

        // Set frame
        CurrentFrame = startingFrame;

        // Mark as not spritesheet
        m_spriteSheet = false;
    }

    void Sprite::draw(graphics::Renderer *renderer, Vector2 position,
                      float rotation, Vector2 origin) {
        if (m_textures.empty())
            return;

        getCurrentTexture()->draw(
            renderer, {position, (float)DrawWidth, (float)DrawHeight},
            getSourceRectangle(), Color::White, origin, rotation);
    }

    void Sprite::update() {
        if (isAnimated()) {
            // Increment timer
            m_animationTimer++;

            while (m_animationTimer > 0 &&
                   fmod(m_animationTimer, FrameTime) == 0) {
                // Reset timer
                m_animationTimer = 0;

                // Increase frame
                CurrentFrame++;

                // Reset if out of range
                if (m_spriteSheet) {
                    auto count = (getCurrentTexture()->Width / FrameWidth) *
                                 (getCurrentTexture()->Height / FrameHeight);

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
                return FrameHeight != getCurrentTexture()->Height ||
                       FrameWidth != getCurrentTexture()->Width;
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

    void Sprite::setTexture(Texture2D *texture) {
        m_textures.clear();
        m_textures.push_back(texture);
    }

    void Sprite::setTextures(const std::vector<Texture2D *> &textures) {
        m_textures.clear();

        for (auto t : textures) {
            // Look for existing shared pointer first.
            m_textures.push_back(t);
        }
    }

    Rectangle Sprite::getSourceRectangle() {
        if (m_spriteSheet)
            return {static_cast<float>(getFrameX()),
                    static_cast<float>(getFrameY()),
                    static_cast<float>(FrameWidth),
                    static_cast<float>(FrameHeight)};
        else
            return {0, 0, static_cast<float>(getCurrentTexture()->Width),
                    static_cast<float>(getCurrentTexture()->Height)};
    }

    bool Sprite::operator==(const Sprite &b) {
        return m_textures == b.m_textures && DrawHeight == b.DrawHeight &&
               DrawWidth == b.DrawWidth && FrameHeight == b.FrameHeight &&
               FrameWidth == b.FrameWidth && FrameTime == b.FrameTime;
    }

    bool Sprite::operator!=(const Sprite &b) {
        return m_textures != b.m_textures || DrawHeight != b.DrawHeight ||
               DrawWidth != b.DrawWidth || FrameHeight != b.FrameHeight ||
               FrameWidth != b.FrameWidth || FrameTime != b.FrameTime;
    }
} // namespace ngine::graphics
