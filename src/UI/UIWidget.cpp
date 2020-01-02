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

#include "UIWidget.hpp"

namespace NerdThings::Ngine::UI {
    // Destructor

    UIWidget::~UIWidget() {
        delete _Panel;
    }

    // Public Method(s)

    void UIWidget::Draw(Graphics::Renderer *renderer_) {
        if (_Panel == nullptr)
            throw std::runtime_error("Panel is null...");
        else
            _Panel->Draw(renderer_);
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