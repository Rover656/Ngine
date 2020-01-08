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

#ifndef UICONTROL_HPP
#define UICONTROL_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "UIStyle.hpp"

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
        virtual void Draw(Graphics::Renderer *renderer_);

        /*
         * Draw the children
         */
        void DrawChildren(Graphics::Renderer *renderer_);

        /*
         * Draw the style instructions
         */
        void DrawStyles(Graphics::Renderer *renderer_);

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

#endif //UICONTROL_HPP