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

#ifndef UICONTROL_H
#define UICONTROL_H

#include "../ngine.h"

#include "../Math/Vector2.h"
#include "UIStyle.h"

namespace NerdThings::Ngine::UI {
    class NEAPI UIPanel;

    /*
     * A UI component, parented by a panel
     */
    class NEAPI UIControl {
        // Private Fields

        /*
         * Globally defined styles
         */
        static std::unordered_map<std::type_index, TUIStyle> _GlobalStyles;

        /*
         * Panel parent, may be null
         */
        UIPanel *_Parent;

        /*
         * The currently applied style
         */
        TUIStyle _Style;
    public:

        // Public Methods

        /*
         * Draw the control
         */
        virtual void Draw();

        /*
         * Get a global style for a control
         */
        static TUIStyle GetGlobalStyle(std::type_index type_);

        /*
         * Get height of control (no padding applied).
         */
        virtual float GetHeight() = 0;

        /*
         * Get parent as type
         */
        template<typename ParentType = UIPanel>
        ParentType* GetParent() {
            return dynamic_cast<ParentType*>(_Parent);
        }

        /*
         * Set a global style, will not be applied to existing controls
         */
        void SetGlobalStyle(std::type_index type_, TUIStyle style_);

        /*
         * Get the screen position.
         * This has the panel offset applied.
         */
        virtual Math::TVector2 GetScreenPosition();

        /*
         * Get the screen rectangle
         */
        Math::TRectangle GetScreenRectangle();

        /*
         * Get the current style
         */
        TUIStyle GetStyle();

        /*
         * Get width of control (no padding applied).
         */
        virtual float GetWidth() = 0;

        /*
         * Set parent panel.
         * Intended for internal use only.
         */
        void InternalSetParent(UIPanel *parent_);

        /*
         * Set the style
         */
        virtual void SetStyle(TUIStyle style_);

        /*
         * Run update logic on the control
         */
        virtual void Update();
    };
}

#endif // UICONTROL_H