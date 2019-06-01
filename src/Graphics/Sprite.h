/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef SPRITE_H
#define SPRITE_H

#include "../ngine.h"
#include "Drawing.h"

namespace NerdThings::Ngine::Graphics {
    class NEAPI Sprite {
        // Private Fields

        /*
         * The animation timer
         */
        float _AnimationTimer = 0;

        /*
         * The textures associated
         */
        std::vector<TTexture2D*> _Textures;

        /*
         * Whether or not this is a spritesheet
         */
        bool _SpriteSheet = true;
    public:

        // Public Fields

        /*
         * The current frame
         */
        int CurrentFrame = 0;

        /*
         * Height to draw with
         */
        int DrawHeight = 0;

        /*
         * Width to draw with
         */
        int DrawWidth = 0;

        /*
         * Animated frame width
         */
        int FrameWidth = 0;

        /*
         * Animated frame height
         */
        int FrameHeight = 0;

        /*
         * Animation Speed
         */
        float ImageSpeed = 0;

        // Public Constructor(s)

        /*
         * Null sprite
         */
        Sprite() = default;

        /*
         * Create a sprite
         */
        Sprite(TTexture2D *texture_);

        /*
         * Create an animated sprite sheet
         */
        Sprite(TTexture2D *texture_, int frameWidth_, int frameHeight_, int drawWidth_, int drawHeight_,
               float imageSpeed_ = 30, int startingFrame = 0);

        /*
         * Create an animated sequence of textures
         */
        Sprite(std::vector<TTexture2D*> textures_, float imageSpeed_ = 30, int startingFrame_ = 0);

        // Destructor

        virtual ~Sprite() = default;

        // Public Methods

        /*
         * Draw the sprite
         */
        void Draw(TVector2 position_, float rotation_, TVector2 origin_ = TVector2::Zero);

        /*
         * Get the current X coordinate
         */
        int FrameX();

        /*
         * Get the current frame Y coordinate
         */
        int FrameY();

        /*
         * Get the current texture
         */
        TTexture2D *GetCurrentTexture();

        /*
         * Get the source rectangle for drawing with
         */
        TRectangle GetSourceRectangle();

        /*
         * Whether or not the sprite is animated
         */
        bool IsAnimated();

        /*
         * Update the sprite timer
         * Should be called every frame
         */
        void Update();
    };
}

#endif //SPRITE_H
