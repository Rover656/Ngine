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

#ifndef UICONTROLSIZED_HPP
#define UICONTROLSIZED_HPP

#include "../Config.hpp"

#include "UIControl.hpp"

namespace NerdThings::Ngine::UI {
    /*
     * A control that has a fixed size
     */
    class NEAPI UIControlSized : virtual public UIControl {
        // Private Fields

        /*
         * Whether or not max constraints are set
         */
        bool _ConstraintsDefined = false;

        /*
         * Constrain dimensions to parent panel
         */
        bool _ConstrainToPanel = false;

        /*
         * The control height
         */
        float _Height = 0;

        /*
         * Maximum height
         */
        float _MaxHeight = 0;

        /*
         * Maximum width
         */
        float _MaxWidth = 0;

        /*
         * Minimum height
         */
        float _MinHeight = 0;

        /*
         * Minimum width
         */
        float _MinWidth = 0;

        /*
         * The control width
         */
        float _Width = 0;

        // Private Methods

        /*
         * Ensure size constraints are enforced
         */
        void CheckConstraints();
    public:

        // Public Methods

        /*
         * Determine if the dimensions are constrained to the panel
         */
        bool GetConstrainToPanel();

        /*
         * Get maximum height.
         * Returns 0 if constraints aren't defined.
         */
        float GetMaxHeight();

        /*
         * Get maximum width.
         * Returns 0 if constraints aren't defined.
         */
        float GetMaxWidth();

        /*
         * Get minumum height.
         */
        float GetMinHeight();

        /*
         * Get minimum width.
         */
        float GetMinWidth();

        /*
         * Get height
         */
        float GetHeight() override;

        /*
         * Get width
         */
        float GetWidth() override;

        /*
         * Set whether or not this should be constrained within parent panel size.
         */
        void SetConstrainToPanel(bool constrain_);

        /*
         * Set max height.
         */
        virtual void SetMaxHeight(float height_);

        /*
         * Set max width.
         */
        virtual void SetMaxWidth(float width_);

        /*
         * Set min height.
         */
        virtual void SetMinHeight(float height_);

        /*
         * Set min width.
         */
        virtual void SetMinWidth(float width_);

        /*
         * Set height.
         */
        virtual void SetHeight(float height_);

        /*
         * Set width.
         */
        virtual void SetWidth(float width_);
    };
}

#endif //UICONTROLSIZED_HPP
