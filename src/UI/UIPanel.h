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

#ifndef UIPANEL_H
#define UIPANEL_H

#include "../ngine.h"

#include "../Math/Vector2.h"
#include "UIControl.h"
#include "UIControlSized.h"

namespace NerdThings::Ngine::UI {
    class NEAPI UIWidget;

    /*
     * A UI Panel base, contains entities
     */
    class NEAPI UIPanel : public UIControlSized {
        // Private Fields

        /*
         * Panel children
         */
        std::unordered_map<std::string, UIControl *> _Children;

        /*
         * Children stored in a vector that is ordered on add time.
         */
        std::vector<UIControl *> _ChildrenOrdered;

        /*
         * Parent widget (If this is the root panel)
         */
        UIWidget *_ParentWidget = nullptr;
    public:

        // Public Methods

        /*
         * Draw the panel
         */
        void Draw() override;

        /*
         * Add a child to the panel
         */
        void AddChild(std::string name, UIControl *control_);

        /*
         * Focus the next element
         */
        virtual void FocusNext() = 0;

        /*
         * Focus the last element.
         */
        virtual void FocusPrev() = 0;

        /*
         * Get child by name and type
         */
        template<typename ControlType>
        ControlType *GetChild(const std::string &name) {
            return dynamic_cast<ControlType *>(_Children[name]);
        }

        std::vector<UIControl *> GetChildren() {
            return _ChildrenOrdered;
        }

        /*
         * Get positional offset on screen
         */
        Math::TVector2 GetOffset();

        /*
         * Get space above a control
         */
        float GetOffsetAbove(std::string &name_);

        /*
         * Get space above a control
         */
        virtual float GetOffsetAbove(UIControl *control_) = 0;

        /*
         * Get space beside a control
         */
        float GetOffsetBeside(std::string &name_);

        /*
         * Get space beside a control
         */
        virtual float GetOffsetBeside(UIControl *control_) = 0;

        /*
         * Get the panel position
         */
        Math::TVector2 GetScreenPosition() override;

        /*
         * Set the parent widget.
         * For internal use only.
         */
        void InternalSetParentWidget(UIWidget *widget_);

        /*
         * Remove a child control
         */
        void RemoveChild(std::string name);

        /*
         * Update the panel
         */
        void Update() override;

    protected:

        // Protected Constructor(s)

        /*
         * Create a UI Panel
         */
        UIPanel(float width_, float height_);
    };
}

#endif // UIPANEL_H