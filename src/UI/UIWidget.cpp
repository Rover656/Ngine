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
    // Destructor

    UIWidget::~UIWidget() {
        delete _Panel;
    }

    // Public Method(s)

    void UIWidget::Draw() {
        if (_Panel == nullptr)
            throw std::runtime_error("Panel is null...");
        else
            _Panel->Draw();
    }

    Vector2 UIWidget::GetPosition() {
        return _Position;
    }

    void UIWidget::SetPanel(UIPanel *panel_) {
        _Panel = panel_;
        _Panel->InternalSetParentWidget(this);
    }

    void UIWidget::SetPosition(Vector2 pos_) {
        _Position = pos_;
    }

    void UIWidget::Update() {
        if (_Panel != nullptr)
            _Panel->Update();
    }
}