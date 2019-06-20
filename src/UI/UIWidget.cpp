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

#include "UIWidget.h"

namespace NerdThings::Ngine::UI {
    // Public Constructor(s)

    UIWidget::UIWidget(Math::TVector2 pos_, UIPanel *panel_)
            : _Position(pos_), _Panel(panel_) {}

    void UIWidget::Draw() {

    }

    UIPanel* UIWidget::GetPanel() {

    }

    Math::TVector2 UIWidget::GetPosition() {

    }

    void UIWidget::SetPosition(Math::TVector2 pos_) {

    }

    void UIWidget::Update() {

    }
}