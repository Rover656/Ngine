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

#ifndef UIWIDGETCOMPONENT_HPP
#define UIWIDGETCOMPONENT_HPP

#include "../Config.hpp"

#include "../UI/UIWidget.h"
#include "../Entity.hpp"
#include "../Component.hpp"

// Not being documented as this is outdated.
// UI engine will be replaced, then this will be fixed.

namespace NerdThings::Ngine::Components {
    class UIWidgetComponent : public Component {
        /*
         * On position changed
         */
        EventAttachment<EntityTransformChangedEventArgs> m_onTransformChangeRef;

        /*
         * The UI widget we are attached to.
         */
        UI::UIWidget m_widget;
    public:

        // Public Constructor(s)

        UIWidgetComponent(Entity *parent_, const UI::UIWidget& widget_)
                : Component(parent_), m_widget(widget_) {
            SubscribeToDraw();
            SubscribeToUpdate();

            m_onTransformChangeRef = GetEntity()->OnTransformChanged +=
                    new ClassMethodEventHandler<UIWidgetComponent, EntityTransformChangedEventArgs>
                            (this, &UIWidgetComponent::UpdatePosition);
        }

        // Destructor

        ~UIWidgetComponent() {
            m_onTransformChangeRef.Detach();
        }

        // Public Methods

        void Draw() override {
            m_widget.Draw();
        }

        void Update() override {
            m_widget.Update();
        }

        void UpdatePosition(EntityTransformChangedEventArgs e) {
            m_widget.SetPosition(e.EntityPosition);
        }
    };
}

#endif //UIWIDGETCOMPONENT_HPP
