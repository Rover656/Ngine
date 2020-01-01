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

#ifndef UIWIDGET_HPP
#define UIWIDGET_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "UIPanel.hpp"

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

#endif //UIWIDGET_HPP