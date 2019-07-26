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

#include "../Graphics/RenderTarget.h"
#include "../Graphics/GraphicsManager.h"
#include "../Vector2.h"
#include "UIControl.h"
#include "UIControlSized.h"

namespace NerdThings::Ngine::UI {
    class NEAPI UIWidget;

    /*
     * A UI Panel base, contains entities
     */
    class NEAPI UIPanel : virtual public UIControlSized {
        // Private Fields

        /*
         * Parent widget (If this is the root panel)
         */
        UIWidget *_ParentWidget = nullptr;

        /*
         * The panel render target
         */
        std::shared_ptr<Graphics::TRenderTarget> _RenderTarget = nullptr;
    public:

        // Destructor

        virtual ~UIPanel();

        // Public Methods

        /*
         * Draw the panel
         */
        void Draw() override;

        /*
         * Focus the next element
         */
        virtual void FocusNext() = 0;

        /*
         * Focus the last element.
         */
        virtual void FocusPrev() = 0;

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
         * Get the position of the panel on screen
         */
        TVector2 GetLogicPosition();

        /*
         * Gets the panel position relative to the render target (0, 0)
         */
        TVector2 GetRenderPosition() override;

        /*
         * Set the parent widget.
         * For internal use only.
         */
        void InternalSetParentWidget(UIWidget *widget_);

        /*
         * Set panel height
         */
        void SetHeight(float height_) override;

        /*
         * Set panel width
         */
        void SetWidth(float width_) override;

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