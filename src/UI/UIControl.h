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

#ifndef UICONTROL_H
#define UICONTROL_H

#include "../Config.hpp"

#include "../Math.hpp"
#include "UIStyle.h"

namespace NerdThings::Ngine::UI {
    class NEAPI UIPanel;

    ////////////////////
    // TODO: Add a way for a control to resize to the size of its children (optional)
    ///////////////////

    /*
     * A UI component, parented by a panel
     */
    class NEAPI UIControl {
        // Private Fields

        /*
         * Children
         */
        std::unordered_map<std::string, UIControl *> _Children;

        /*
         * Children stored in a vector that is ordered on add time.
         */
        std::vector<UIControl *> _ChildrenOrdered;

        /*
         * Globally defined styles
         */
        static std::unordered_map<std::type_index, TUIStyle> _GlobalStyles;

        /*
         * Control parent, may be null
         */
        UIControl *_Parent = nullptr;

        /*
         * The currently applied style
         */
        TUIStyle _Style;
    public:

        // Destructor

        virtual ~UIControl();

        // Public Methods

        /*
         * Add a child
         */
        void AddChild(std::string name, UIControl *control_);

        /*
         * Draw the control
         */
        virtual void Draw();

        /*
         * Draw the children
         */
        void DrawChildren();

        /*
         * Draw the style instructions
         */
        void DrawStyles();

        /*
         * Get child by name and type
         */
        template<typename ControlType>
        ControlType *GetChild(const std::string &name) {
            return dynamic_cast<ControlType *>(_Children[name]);
        }

        /*
         * Get all of the children in add order
         */
        std::vector<UIControl *> GetChildren() {
            return _ChildrenOrdered;
        }

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
        template<typename ParentType>
        ParentType* GetParent() {
            return dynamic_cast<ParentType*>(_Parent);
        }

        /*
         * Set a global style, will not be applied to existing controls
         */
        void SetGlobalStyle(std::type_index type_, TUIStyle style_);

        /*
         * Get the render position.
         */
        virtual Vector2 GetLogicPosition();

        /*
         * Get the render rectangle
         */
        Rectangle GetLogicRectangle();

        /*
         * Get the render position.
         */
        virtual Vector2 GetRenderPosition();

        /*
         * Get the render rectangle
         */
        Rectangle GetRenderRectangle();

        /*
         * Get the current style
         */
        TUIStyle GetStyle();

        /*
         * Get width of control (no padding applied).
         */
        virtual float GetWidth() = 0;

        /*
         * Remove a child control
         */
        void RemoveChild(std::string name);

        /*
         * Set the style
         */
        virtual void SetStyle(TUIStyle style_);

        /*
         * Run update logic on the control
         */
        virtual void Update();
    protected:
        // Protected Fields

        /*
         * Children rules.
         * 0 - No children
         * 1 - 1 Child
         * 2 - Many children
         */
        char _ChildrenConfig = 0;
    };
}

#endif // UICONTROL_H