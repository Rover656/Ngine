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

#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include "../../Ngine.h"

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
        Graphics::Sprite _Sprite;

    public:

        // Public Constructor(s)

        /*
         * Create a new image control
         */
        Image(Graphics::Sprite sprite_, float width_, float height_) {
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

#endif // IMAGECONTROL_H
