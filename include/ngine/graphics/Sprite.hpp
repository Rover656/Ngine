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

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "Texture2D.hpp"

namespace ngine::graphics {
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
         * The number of game frames before changing animation frame. E.g for 60fps game, setting this to 60 makes one animation frame happen each second.
         */
        float FrameTime = 0;

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
         * @param texture The texture to use.
         */
        Sprite(Texture2D *texture);

        /**
         * Create an animated sprite sheet
         *
         * @param texture The sprite sheet.
         * @param frameWidth The width of each frame.
         * @param frameHeight The height of each frame.
         * @param drawWidth The width to draw with.
         * @param drawHeight The height to draw with.
         * @param imageSpeed The image speed.
         * @param startingFrame The frame to start with.
         */
        Sprite(Texture2D *texture, int frameWidth, int frameHeight, int drawWidth, int drawHeight,
               float imageSpeed = 30, int startingFrame = 0);

        /**
         * Create an animated sequence of textures.
         *
         * @param textures Texture sequence.
         * @param imageSpeed The image speed.
         * @param startingFrame The image to start with.
         */
        Sprite(const std::vector<Texture2D *> &textures, float imageSpeed = 30, int startingFrame = 0);

        virtual ~Sprite() = default;

        /**
         * Draw the sprite.
         *
         * @param position Position to draw at.
         * @param scale Scale to draw with.
         * @param rotation Rotation of the sprite.
         * @param origin Origin of the sprite (in percentages (0 -> 1))
         */
        void draw(graphics::Renderer *renderer, Vector2 position, float rotation = 0,
                  Vector2 origin = {0.5f, 0.5f});

        /**
         * Update the sprite timer.
         * Should be called every frame.
         */
        void update();

        /**
         * Whether or not the sprite is animated.
         *
         * @return Whether the sprite is animated or not.
         */
        bool isAnimated();

        /**
         * Get the current X coordinate.
         *
         * @return The current X coordinate of the frame.
         */
        int getFrameX();

        /**
         * Get the current frame Y coordinate.
         *
         * @return The current Y coordinate of the frame.
         */
        int getFrameY();

        /**
         * Get the current texture.
         *
         * @return The currently active texture.
         */
        Texture2D *getCurrentTexture();

        /**
         * Set the sprite texture.
         *
         * @param texture The texture to use.
         */
        void setTexture(Texture2D *texture);

        /**
         * Set the sprite textures.
         *
         * @param textures The textures to use.
         */
        void setTextures(const std::vector<Texture2D *> &textures);

        /**
         * Get the source rectangle for drawing with.
         *
         * @return The source rectangle for rendering.
         */
        Rectangle getSourceRectangle();

        bool operator==(const Sprite &b);

        bool operator!=(const Sprite &b);
    };
}

#endif //SPRITE_HPP
