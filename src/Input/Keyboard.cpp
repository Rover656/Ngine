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

#include "Keyboard.h"

namespace NerdThings::Ngine::Input {
    // Public Methods

    EKey Keyboard::GetLatestKeypress() {
        //return static_cast<EKey>(GetKeyPressed());
        return KEY_NONE;
    }

    bool Keyboard::IsKeyDown(const EKey key_) {
        //return ::IsKeyDown(static_cast<int>(key_));
        return false;
    }

    bool Keyboard::IsKeyPressed(const EKey key_) {
        //return ::IsKeyPressed(static_cast<int>(key_));
        return false;
    }

    bool Keyboard::IsKeyReleased(const EKey key_) {
        //return ::IsKeyReleased(static_cast<int>(key_));
        return false;
    }

    bool Keyboard::IsKeyUp(const EKey key_) {
        //return ::IsKeyUp(static_cast<int>(key_));
        return true;
    }

    void Keyboard::SetExitKey(const EKey key_) {
        //::SetExitKey(static_cast<int>(key_));
        // TODO: Either use or remove
    }

}
