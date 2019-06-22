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

#include "UIControl.h"

#include "../Graphics/Drawing.h"
#include "UIPanel.h"

namespace NerdThings::Ngine::UI {
    // Private Fields

    std::unordered_map<std::type_index, TUIStyle> UIControl::_GlobalStyles;

    // Public Method(s)

    void UIControl::Draw() {
        // Draw default styles

        auto style = GetStyle();

        auto controlRect = GetScreenRectangle();
        auto controlBorderRect = style.GetBorderRect(controlRect);
        auto controlBackgroundRect = style.GetBackgroundRect(controlRect);

        // Background

        if (style.BackgroundTexture != nullptr)
            Graphics::Drawing::DrawTexture(style.BackgroundTexture,
                                           controlBackgroundRect,
                                           {
                                                   0,
                                                   0,
                                                   static_cast<float>(style.BackgroundTexture->Width),
                                                   static_cast<float>(style.BackgroundTexture->Height)
                                           },
                                           style.BackColor);
        else
            Graphics::Drawing::DrawRectangle(controlBackgroundRect, style.BackColor);

        // Border
        if (style.BorderThickness > 0) {
            Graphics::Drawing::DrawRectangleLines(controlBorderRect, style.BorderColor,
                                                  static_cast<int>(style.BorderThickness));
        }
    }

    TUIStyle UIControl::GetGlobalStyle(std::type_index type_) {
        if (_GlobalStyles.find(type_) != _GlobalStyles.end())
            return _GlobalStyles[type_];
        return TUIStyle();
    }

    void UIControl::SetGlobalStyle(std::type_index type_, TUIStyle style_) {
        if (_GlobalStyles.find(type_) != _GlobalStyles.end())
            _GlobalStyles[type_] = style_;
        else
            _GlobalStyles.insert({type_, style_});
    }

    Math::TVector2 UIControl::GetScreenPosition() {
        // TODO: Cache this
        auto par = GetParent<UIPanel>();
        if (par == nullptr)
            return {0, 0};
        auto pos = par->GetOffset();
        pos.X += par->GetOffsetBeside(this);
        pos.Y += par->GetOffsetAbove(this);
        return pos;
    }

    Math::TRectangle UIControl::GetScreenRectangle() {
        return Math::TRectangle(GetScreenPosition(), GetWidth(), GetHeight());
    }

    TUIStyle UIControl::GetStyle() {
        return _Style;
    }


    void UIControl::InternalSetParent(UIPanel *parent_) {
        _Parent = parent_;
    }

    void UIControl::SetStyle(TUIStyle style_) {
        _Style = style_;
    }

    void UIControl::Update() {

    }
}