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

#ifndef IMAGE_H
#define IMAGE_H

#include "../../ngine.h"

#include "../../Graphics/Sprite.h"
#include "../UIControlSized.h"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A basic image
     */
    class Image : public UIControlSized {
    private:
        // Private Fields

        /*
         * The sprite used for drawing
         */
        Graphics::TSprite _Sprite;

    public:

        // Public Constructor(s)

        /*
         * Create a new image control
         */
        Image(Graphics::TSprite sprite_, float width_, float height_) {
            _Sprite = sprite_;
            SetWidth(width_);
            SetHeight(height_);
        }

        // Public Methods

        void Draw() override {
            UIControl::Draw();

            auto style = GetStyle();
            auto controlContentRect = style.GetContentRect(GetRenderRectangle());

            _Sprite.Draw({controlContentRect.X, controlContentRect.Y}, 0);
        }

        void Update() override {
            UIControl::Update();

            auto style = GetStyle();
            auto controlContentRect = style.GetContentRect(GetRenderRectangle());
            _Sprite.DrawWidth = controlContentRect.Width;
            _Sprite.DrawHeight = controlContentRect.Height;

            _Sprite.Update();
        }
    };
};

#endif // IMAGE_H
