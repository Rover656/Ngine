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

#include "UIPanel.h"

#include "../Graphics/Drawing.h"
#include "UIWidget.h"

namespace NerdThings::Ngine::UI {
    // Public Methods

    void UIPanel::AddChild(std::string name, UIControl *control_) {
        if (_Children.find(name) != _Children.end())
            throw std::runtime_error("Cannot add a child by the same name.");
        _Children.insert({name, control_});
        _ChildrenOrdered.push_back(control_);
        control_->InternalSetParent(this);
    }

    void UIPanel::Draw() {
        UIControl::Draw();

        for (auto pair : _Children) {
            pair.second->Draw();
        }
    }

    Math::TVector2 UIPanel::GetOffset() {
        // TODO: Cache this

        auto style = GetStyle();
        if (_ParentWidget != nullptr) {
            return style.GetContentPosition(_ParentWidget->GetPosition());
        } else {
            auto par = GetParent<UIPanel>();
            auto parOff = par->GetOffset();
            parOff.X += par->GetOffsetBeside(this);
            parOff.Y += par->GetOffsetAbove(this);
            return style.GetContentPosition(parOff);
        }
    }

    float UIPanel::GetOffsetAbove(std::string &name_) {
        return GetOffsetAbove(GetChild<UIControl>(name_));
    }

    float UIPanel::GetOffsetBeside(std::string &name_) {
        return GetOffsetBeside(GetChild<UIControl>(name_));
    }

    void UIPanel::InternalSetParentWidget(UIWidget *widget_) {
        _ParentWidget = widget_;
    }

    void UIPanel::RemoveChild(std::string name) {
        if (_Children.find(name) == _Children.end())
            throw std::runtime_error("Cannot remove a child that does not exist.");
        auto child = _Children[name];
        child->InternalSetParent(nullptr);
        _ChildrenOrdered.erase(std::remove(_ChildrenOrdered.begin(), _ChildrenOrdered.end(), child), _ChildrenOrdered.end());
        _Children.erase(name);
    }

    void UIPanel::Update() {
        UIControl::Update();

        for (auto pair : _Children) {
            pair.second->Update();
        }
    }

    // Protected Constructor(s)

    UIPanel::UIPanel(float width_, float height_) {
        SetHeight(height_);
        SetWidth(width_);
    }

    Math::TVector2 UIPanel::GetScreenPosition() {
        if (_ParentWidget != nullptr)
            return _ParentWidget->GetPosition();
        else {
            return UIControl::GetScreenPosition();
        }
    }
}