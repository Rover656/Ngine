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

#include "UIPanel.hpp"

#include "../Graphics/Renderer.hpp"
#include "UIWidget.hpp"

namespace NerdThings::Ngine::UI {
    // Destructor

    UIPanel::~UIPanel() {
    }

    // Public Methods

    void UIPanel::Draw(Graphics::Renderer *renderer_) {
        DrawStyles(renderer_);

#ifndef USE_EXPERIMENTAL_RENDERER
        Graphics::GraphicsManager::PushTarget(_RenderTarget.get());
        renderer_->Clear(Graphics::Color::Transparent);
        DrawChildren(renderer_);
        Graphics::GraphicsManager::PopTarget();

        auto rPos = GetRenderPosition();

        // Draw target
        _RenderTarget->GetTexture()->Draw(
                renderer_,
                {
                        rPos.X,
                        rPos.Y,
                        GetWidth(),
                        GetHeight()
                },
                {
                        0,
                        0,
                        static_cast<float>(_RenderTarget->Width),
                        static_cast<float>(_RenderTarget->Height) * -1
                },
                Graphics::Color::White);
#endif
    }

    float UIPanel::GetOffsetAbove(std::string &name_) {
        return GetOffsetAbove(GetChild<UIControl>(name_));
    }

    float UIPanel::GetOffsetBeside(std::string &name_) {
        return GetOffsetBeside(GetChild<UIControl>(name_));
    }

    Vector2 UIPanel::GetLogicPosition() {
        if (_ParentWidget != nullptr)
            return _ParentWidget->GetPosition();
        else
            return UIControl::GetLogicPosition();
    }

    Vector2 UIPanel::GetRenderPosition() {
        if (_ParentWidget != nullptr)
            return _ParentWidget->GetPosition();
        else
            return UIControl::GetRenderPosition();
    }

    void UIPanel::InternalSetParentWidget(UIWidget *widget_) {
        _ParentWidget = widget_;
    }

    void UIPanel::SetHeight(float height_) {
        UIControlSized::SetHeight(height_);

        _RenderTarget = std::make_shared<Graphics::RenderTarget>(static_cast<int>(GetWidth()),
                                                                 static_cast<int>(height_));
    }

    void UIPanel::SetWidth(float width_) {
        UIControlSized::SetWidth(width_);

        _RenderTarget = std::make_shared<Graphics::RenderTarget>(static_cast<int>(width_),
                                                                 static_cast<int>(GetHeight()));
    }

    void UIPanel::Update() {
        UIControl::Update();
    }

    // Protected Constructor(s)

    UIPanel::UIPanel(float width_, float height_) {
        SetHeight(height_);
        SetWidth(width_);
        _ChildrenConfig = 3; // Allow multiple children

        // Create render target
        _RenderTarget = std::make_shared<Graphics::RenderTarget>(static_cast<int>(width_), static_cast<int>(height_));
    }
}