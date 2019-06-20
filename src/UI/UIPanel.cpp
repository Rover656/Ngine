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

namespace NerdThings::Ngine::UI {
    // Public Methods

    void UIPanel::AddChild(std::string name, UIControl *control_) {
        if (_Children.find(name) != _Children.end())
            throw std::runtime_error("Cannot add a child by the same name.");
        _Children.insert({name, control_});
    }

    void UIPanel::RemoveChild(std::string name) {
        if (_Children.find(name) == _Children.end())
            throw std::runtime_error("Cannot remove a child that does not exist.");
        _Children.erase(name);
    }
}