/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Sprite.h"

#include <cmath>

#include "../Graphics/Color.h"
#include "../Rectangle.h"
#include "Renderer.h"

namespace NerdThings::Ngine::Graphics {
    Sprite::Sprite(Texture2D *texture_) {
        // Add texture
        _Textures.push_back(texture_);

        // Set draw dimensions
        DrawHeight = texture_->Height;
        DrawWidth = texture_->Width;

        // Mark as spritesheet
        _SpriteSheet = true;
    }

    Sprite::Sprite(Texture2D *texture_, int frameWidth_, int frameHeight_, int drawWidth_, int drawHeight_,
                   float imageSpeed_, int startingFrame)
        : DrawHeight(drawHeight_), DrawWidth(drawWidth_), FrameWidth(frameWidth_), FrameHeight(frameHeight_),
          ImageSpeed(imageSpeed_) {
        // Add texture
        _Textures.push_back(texture_);

        // Set frame
        CurrentFrame = startingFrame;

        // Mark as spritesheet
        _SpriteSheet = true;
    }

    Sprite::Sprite(const std::vector<Texture2D*> &textures_, float imageSpeed_, int startingFrame_) {
        // Set textures
        for (auto tex : textures_) {
            _Textures.push_back(tex);
        }

        // Set frame
        CurrentFrame = startingFrame_;

        // Mark as not spritesheet
        _SpriteSheet = false;
    }

    void Sprite::Draw(Vector2 position_, float scale_, float rotation_, Vector2 origin_) {
        if (_Textures.empty()) return;

        Renderer::DrawTexture(GetCurrentTexture(),
                              Rectangle(
                                 position_,
                                 static_cast<float>(DrawWidth * scale_),
                                 static_cast<float>(DrawHeight * scale_)),
                              GetSourceRectangle(),
                              Color::White,
                              origin_,
                              rotation_);
    }

    void Sprite::Update() {
        if (IsAnimated()) {
            // Increment timer
            _AnimationTimer++;

            while (_AnimationTimer > 0 && fmod(_AnimationTimer, ImageSpeed) == 0) {
                // Reset timer
                _AnimationTimer = 0;

                // Increase frame
                CurrentFrame++;

                // Reset if out of range
                if (_SpriteSheet) {
                    auto count = (GetCurrentTexture()->Width / FrameWidth) * (GetCurrentTexture()->Height / FrameHeight);

                    if (CurrentFrame > count - 1)
                        CurrentFrame = 0;
                } else {
                    if (CurrentFrame > _Textures.size() - 1)
                        CurrentFrame = 0;
                }
            }
        }
    }

    bool Sprite::IsAnimated() {
        if (_SpriteSheet) {
            if (GetCurrentTexture()->IsValid())
                return FrameHeight != GetCurrentTexture()->Height || FrameWidth != GetCurrentTexture()->Width;
            else
                return false;
        } else {
            return _Textures.size() > 1;
        }
    }

    int Sprite::FrameX() {
        if (!_SpriteSheet || _Textures.empty())
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
        if (!_SpriteSheet || _Textures.empty())
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
        if (_Textures.empty())
            return nullptr;

        if (_SpriteSheet) {
            return _Textures[0];
        }

        return _Textures[CurrentFrame];
    }

    void Sprite::SetTexture(Texture2D *texture_) {
        _Textures.clear();
        _Textures.push_back(texture_);
    }

    void Sprite::SetTextures(const std::vector<Texture2D *> &textures_) {
        _Textures.clear();

        for (auto t : textures_) {
            // Look for existing shared pointer first.
            _Textures.push_back(t);
        }
    }

    Rectangle Sprite::GetSourceRectangle() {
        if (_SpriteSheet)
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
        return _Textures == b._Textures && DrawHeight == b.DrawHeight && DrawWidth == b.DrawWidth && FrameHeight == b.FrameHeight && FrameWidth == b.FrameWidth && ImageSpeed == b.ImageSpeed;
    }

    bool Sprite::operator!=(const Sprite &b) {
        return _Textures != b._Textures || DrawHeight != b.DrawHeight || DrawWidth != b.DrawWidth || FrameHeight != b.FrameHeight || FrameWidth != b.FrameWidth || ImageSpeed != b.ImageSpeed;
    }
}
