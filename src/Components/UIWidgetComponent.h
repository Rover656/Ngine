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

#ifndef UIWIDGETCOMPONENT_H
#define UIWIDGETCOMPONENT_H

#include "../Ngine.h"

#include "../UI/UIWidget.h"
#include "../BaseEntity.h"
#include "../Component.h"

namespace NerdThings::Ngine::Components {
    class UIWidgetComponent : public Component {
        // Private Fields

        /*
         * On position changed
         */
        EventHandleRef<EntityTransformChangedEventArgs> _OnTransformChangeRef;

        /*
         * The UI widget we are attached to.
         */
        UI::UIWidget _Widget;
    public:

        // Public Constructor(s)

        UIWidgetComponent(BaseEntity *parent_, const UI::UIWidget& widget_)
                : Component(parent_), _Widget(widget_) {
            SubscribeToDraw();
            SubscribeToUpdate();

            _OnTransformChangeRef = GetParent<BaseEntity>()->OnTransformChanged.Bind(
                    this, &UIWidgetComponent::UpdatePosition);
        }

        // Destructor

        ~UIWidgetComponent() {
            _OnTransformChangeRef.UnBind();
        }

        // Public Methods

        void Draw(EventArgs &e) override {
            _Widget.Draw();
        }

        void Update(EventArgs &e) override {
            _Widget.Update();
        }

        void UpdatePosition(EntityTransformChangedEventArgs &e) {
            _Widget.SetPosition(e.EntityPosition);
        }
    };
}

#endif // UIWIDGETCOMPONENT_H
