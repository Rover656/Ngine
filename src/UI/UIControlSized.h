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

#ifndef UICONTROLSIZED_H
#define UICONTROLSIZED_H

#include "UIControl.h"

namespace NerdThings::Ngine::UI {
    /*
     * A control that has a fixed size
     */
    class UIControlSized : public UIControl {
        // Private Fields

        /*
         * The control height
         */
        float _Height;

        /*
         * The control width
         */
        float _Width;
    public:

        // Public Methods

        float GetHeight() override {
            return _Height;
        }

        float GetWidth() override {
            return _Width;
        }

        virtual void SetHeight(float height_) {
            _Height = height_;
        }

        virtual void SetWidth(float width_) {
            _Width = width_;
        }
    };
}

#endif // UICONTROLSIZED_H
