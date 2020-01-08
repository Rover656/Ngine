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

#ifndef UIPANEL_HPP
#define UIPANEL_HPP

#include "../Config.hpp"

#include "../Graphics/RenderTarget.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Math.hpp"
#include "UIControl.hpp"
#include "UIControlSized.hpp"

namespace NerdThings::Ngine::UI {
    class NEAPI UIWidget;

    /*
     * Horizontal alignment enum
     */
    enum EHorizontalAlignment {
        /*
         * Align elements with the left side
         */
        ALIGN_LEFT = 0,

        /*
         * Align elements with the middle
         */
        ALIGN_CENTER,

        /*
         * Align elements with the right side
         */
        ALIGN_RIGHT
    };

    /*
     * Vertical alignment enum
     */
    enum EVerticalAlignment {
        /*
         * Align elements with top
         */
        ALIGN_TOP = 0,

        /*
         * Align elements with middle
         */
        ALIGN_MIDDLE,

        /*
         * Align elements with bottom
         */
        ALIGN_BOTTOM
    };

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
        std::shared_ptr<Graphics::RenderTarget> _RenderTarget;

        Graphics::GraphicsDevice *m_graphicsDevice = nullptr;
    public:

        UIPanel(Graphics::GraphicsDevice *graphicsDevice_) : UIControl() {
            m_graphicsDevice = graphicsDevice_;
        }

        // Destructor

        virtual ~UIPanel();

        // Public Methods

        /*
         * Draw the panel
         */
        void Draw(Graphics::Renderer *renderer_) override;

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
        Vector2 GetLogicPosition();

        /*
         * Gets the panel position relative to the render target (0, 0)
         */
        Vector2 GetRenderPosition() override;

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

#endif //UIPANEL_HPP