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

#ifndef UIWIDGET_H
#define UIWIDGET_H

#include "../ngine.h"

#include "../Math/Vector2.h"
#include "UIPanel.h"

namespace NerdThings::Ngine::UI {
    /*
     * A simple UI widget, containing panels and controls.
     */
    class NEAPI UIWidget {
        // Private Fields

        /*
         * The panel
         */
        UIPanel *_Panel;

        /*
         * Widget position
         */
        Math::TVector2 _Position;
    public:
        // Public Constructor(s)

        /*
         * Create a UI widget
         */
        UIWidget(Math::TVector2 pos_, UIPanel *panel_);

        // Public Methods

        /*
         * Draw the widget
         */
        void Draw();

        /*
         * Get the panel within the widget
         */
        UIPanel *GetPanel();

        /*
         * Get the position of the widget
         */
        Math::TVector2 GetPosition();

        /*
         * Set the position of the widget
         */
        void SetPosition(Math::TVector2 pos_);

        /*
         * Update the widget
         */
        void Update();
    };
}

#endif //UIWIDGET_H