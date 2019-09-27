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

#include "../Vector2.h"
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
        UIPanel *_Panel = nullptr;

        /*
         * Widget position
         */
        Vector2 _Position;
    public:
        // Public Constructor(s)

        /*
         * Create a UI widget
         */
        UIWidget(Vector2 pos_)
                : _Position(pos_) {}

        // Destructor

        virtual ~UIWidget();

        // Public Methods

        /*
         * Draw the widget
         */
        void Draw();

        /*
         * Get the panel within the widget
         */
        template<typename PanelType = UIPanel>
        PanelType *GetPanel() {
            return dynamic_cast<PanelType *>(_Panel);
        }

        /*
         * Get the position of the widget
         */
        Vector2 GetPosition();

        /*
         * Set the panel
         */
        void SetPanel(UIPanel *panel_);

        /*
         * Set the position of the widget
         */
        void SetPosition(Vector2 pos_);

        /*
         * Update the widget
         */
        void Update();
    };
}

#endif //UIWIDGET_H