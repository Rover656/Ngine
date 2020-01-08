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

#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include "../../Config.hpp"

#include "../../Graphics/Sprite.hpp"
#include "UI/UIControlSized.hpp"

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
