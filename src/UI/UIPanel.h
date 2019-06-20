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

namespace NerdThings::Ngine::UI {
    /*
     * A UI Panel base, contains entities
     */
    class NEAPI UIPanel : public UIControl {
        // Private Fields

        /*
         * Panel children
         */
        std::unordered_map<std::string, UIControl *> _Children;
    public:
        // Public Methods

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

        /*
         * Remove a child control
         */
        void RemoveChild(std::string name);

    protected:

        // Protected Constructor(s)

        /*
         * Create a UI Panel
         */
        UIPanel();
    };
}

#endif // UIPANEL_H