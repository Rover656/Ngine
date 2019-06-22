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

#ifndef UISTYLE_H
#define UISTYLE_H

#include "../ngine.h"

#include "../Graphics/Color.h"
#include "../Graphics/Texture2D.h"
#include "../Math/Rectangle.h"

namespace NerdThings::Ngine::UI {
    /*
     * UI Styling information.
     * Some fields may not apply to some controls
     */
    struct NEAPI TUIStyle {
        // Public Fields

        /*
         * Background color
         */
        Graphics::TColor BackColor = Graphics::TColor::Transparent;

        /*
         * The background texture
         */
        Graphics::TTexture2D *BackgroundTexture = nullptr;

        /*
         * Border color
         */
        Graphics::TColor BorderColor = Graphics::TColor::Gray;

        /*
         * Border thickness
         */
        float BorderThickness = 0;

        /*
         * Foreground color
         */
        Graphics::TColor ForeColor = Graphics::TColor::Black;

        /*
         * Margins,
         * Up, Right, Down, Left
         */
        float Margin[4] = {0,0,0,0};

        /*
         * Padding,
         * Up, Right, Down, Left
         */
        float Padding[4] = {0, 0, 0, 0};

        // Public Methods

        /*
         * Must be passed the border position
         */
        Math::TVector2 GetBackgroundPosition(Math::TVector2 pos_) {
            // Apply border
            pos_ += BorderThickness;

            return pos_;
        }

        Math::TRectangle GetBackgroundRect(Math::TRectangle controlRect_) {
            // Apply border
            controlRect_.X += BorderThickness;
            controlRect_.Y += BorderThickness;

            return controlRect_;
        }

        /*
         * Get the dimensions of the border
         */
        Math::TVector2 GetBorderDimensions(Math::TVector2 dimensions_) {
            // Apply border
            dimensions_ += BorderThickness * 2;

            return dimensions_;
        }

        /*
         * Get the border rectangle
         */
        Math::TRectangle GetBorderRect(Math::TRectangle controlRect_) {
            // Apply border
            controlRect_.Width += BorderThickness * 2;
            controlRect_.Height += BorderThickness * 2;

            // Apply margin TODO: Check this is right
            controlRect_.X += Margin[3];
            controlRect_.Y += Margin[0];

            return controlRect_;
        }

        /*
         * Get the position of the border
         */
        Math::TVector2 GetBorderPosition(Math::TVector2 pos_) {
            // Apply margin TODO: Check this is right
            pos_.X += Margin[3];
            pos_.Y += Margin[0];

            return pos_;
        }

        /*
         * The size of the content
         */
        Math::TVector2 GetContentDimensions(Math::TVector2 dimensions_) {
            // Apply padding
            dimensions_.X -= Padding[1];
            dimensions_.X -= Padding[3];
            dimensions_.Y -= Padding[2];
            dimensions_.Y -= Padding[0];

            return dimensions_;
        }

        /*
         * Get the start position for content inside the control.
         */
        Math::TVector2 GetContentPosition(Math::TVector2 pos_) {
            // Apply border
            pos_ += BorderThickness;

            // Apply padding
            pos_.X += Padding[3];
            pos_.Y += Padding[0];

            // Apply margin TODO: Check this is right
            pos_.X += Margin[3];
            pos_.Y += Margin[0];

            return pos_;
        }

        Math::TRectangle GetContentRect(Math::TRectangle controlRect_) {
            // Apply border
            controlRect_.X += BorderThickness;
            controlRect_.Y += BorderThickness;

            // Apply padding
            controlRect_.X += Padding[3];
            controlRect_.Y += Padding[0];
            controlRect_.Width -= Padding[1];
            controlRect_.Width -= Padding[3];
            controlRect_.Height -= Padding[2];
            controlRect_.Height -= Padding[0];

            // Apply margin TODO: Check this is right
            controlRect_.X += Margin[3];
            controlRect_.Y += Margin[0];

            return controlRect_;
        }
    };
}

#endif // UISTYLE_H
