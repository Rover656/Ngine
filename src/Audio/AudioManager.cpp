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

#include "AudioManager.h"

namespace NerdThings::Ngine::Audio {
    // Private Fields

    bool AudioManager::_Initialized = false;

    // Public Methods

    void AudioManager::CloseDevice() {
        CloseAudioDevice();
    }

    void AudioManager::InitDevice() {
        InitAudioDevice();
    }

}
