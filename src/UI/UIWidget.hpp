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
        void Draw(Graphics::Renderer *renderer_);

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