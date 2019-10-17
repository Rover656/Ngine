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

#ifndef SPRITE_H
#define SPRITE_H

#include "../Ngine.h"

#include "../Rectangle.h"
#include "../Vector2.h"
#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A sprite which may be animated
     */
    class NEAPI Sprite {
        // Private Fields

        /*
         * The animation timer
         */
        float _AnimationTimer = 0;

        /*
         * The textures associated
         */
        std::vector<Texture2D *> _Textures;

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
        Sprite(Texture2D *texture_);

        /*
         * Create an animated sprite sheet
         */
        Sprite(Texture2D *texture_, int frameWidth_, int frameHeight_, int drawWidth_, int drawHeight_,
               float imageSpeed_ = 30, int startingFrame = 0);

        /*
         * Create an animated sequence of textures
         */
        Sprite(const std::vector<Texture2D *> &textures_, float imageSpeed_ = 30, int startingFrame_ = 0);

        // Destructor

        virtual ~Sprite() = default;

        // Public Methods

        /*
         * Draw the sprite
         */
        void Draw(Vector2 position_, float rotation_, float scale_ = 1, Vector2 origin_ = Vector2::Zero);

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
        Texture2D *GetCurrentTexture();

        /*
         * Get the source rectangle for drawing with
         */
        Rectangle GetSourceRectangle();

        /*
         * Whether or not the sprite is animated
         */
        bool IsAnimated();

        /*
         * Set the sprite texture
         */
        void SetTexture(Texture2D *texture_);

        /*
         * Set the sprite textures
         */
        void SetTextures(const std::vector<Texture2D*> &textures_);

        /*
         * Update the sprite timer
         * Should be called every frame
         */
        void Update();

        // Operators

        bool operator==(const Sprite &b);
        bool operator!=(const Sprite &b);
    };
}

#endif //SPRITE_H
