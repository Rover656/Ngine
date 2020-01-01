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

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "Texture2D.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * A sprite is a wrapper which controls the behaviour of a texture or textures.
     */
    class NEAPI Sprite {
        /**
         * The animation timer
         */
        float m_animationTimer = 0;

        /**
         * The textures associated with the sprite.
         */
        std::vector<Texture2D *> m_textures;

        /**
         * Whether or not this is a spritesheet.
         */
        bool m_spriteSheet = true;
    public:
        /**
         * The current animation frame.
         */
        int CurrentFrame = 0;

        /**
         * The width of each frame.
         */
        int FrameWidth = 0;

        /**
         * The height of each frame.
         */
        int FrameHeight = 0;

        /**
         * Animation Speed.
         *
         * @todo Rename this to reflect what it does better. It currently defines how long each frame lasts.
         */
        float ImageSpeed = 0;

        /**
         * Width to draw the sprite with.
         */
        int DrawWidth = 0;

        /**
         * Height to draw the sprite with.
         */
        int DrawHeight = 0;

        /**
         * Create a null sprite.
         */
        Sprite() = default;

        /**
         * Create a sprite.
         *
         * @param texture_ The texture to use.
         */
        Sprite(Texture2D *texture_);

        /**
         * Create an animated sprite sheet
         *
         * @param texture_ The sprite sheet.
         * @param frameWidth_ The width of each frame.
         * @param frameHeight_ The height of each frame.
         * @param drawWidth_ The width to draw with.
         * @param drawHeight_ The height to draw with.
         * @param imageSpeed_ The image speed.
         * @param startingFrame The frame to start with.
         */
        Sprite(Texture2D *texture_, int frameWidth_, int frameHeight_, int drawWidth_, int drawHeight_,
               float imageSpeed_ = 30, int startingFrame = 0);

        /**
         * Create an animated sequence of textures.
         *
         * @param textures_ Texture sequence.
         * @param imageSpeed_ The image speed.
         * @param startingFrame_ The image to start with.
         */
        Sprite(const std::vector<Texture2D *> &textures_, float imageSpeed_ = 30, int startingFrame_ = 0);

        virtual ~Sprite() = default;

        /**
         * Draw the sprite.
         *
         * @param position_ Position to draw at.
         * @param scale_ Scale to draw with.
         * @param rotation_ Rotation of the sprite.
         * @param origin_ Origin of the sprite.
         */
        void Draw(Vector2 position_, float scale_ = 1, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /**
         * Update the sprite timer.
         * Should be called every frame.
         */
        void Update();

        /**
         * Whether or not the sprite is animated.
         *
         * @return Whether the sprite is animated or not.
         */
        bool IsAnimated();

        /**
         * Get the current X coordinate.
         *
         * @return The current X coordinate of the frame.
         */
        int FrameX();

        /**
         * Get the current frame Y coordinate.
         *
         * @return The current Y coordinate of the frame.
         */
        int FrameY();

        /**
         * Get the current texture.
         *
         * @return The currently active texture.
         */
        Texture2D *GetCurrentTexture();

        /**
         * Set the sprite texture.
         *
         * @param texture_ The texture to use.
         */
        void SetTexture(Texture2D *texture_);

        /**
         * Set the sprite textures.
         *
         * @param textures_ The textures to use.
         */
        void SetTextures(const std::vector<Texture2D*> &textures_);

        /**
         * Get the source rectangle for drawing with.
         *
         * @return The source rectangle for rendering.
         */
        Rectangle GetSourceRectangle();

        bool operator==(const Sprite &b);
        bool operator!=(const Sprite &b);
    };
}

#endif //SPRITE_HPP
