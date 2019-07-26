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

#include "Sound.h"

namespace NerdThings::Ngine::Audio {
    // Destructor

    TSound::~TSound() {
//        if (AudioBuffer != nullptr) {
//            UnloadSound(ToRaylibSound());
//            AudioBuffer = nullptr;
//            Buffer = 0;
//            Format = 0;
//            Source = 0;
//        }
    }

    // Public Methods

    std::shared_ptr<TSound> TSound::LoadSound(const std::string &filename_) {
//        const auto snd = ::LoadSound(filename_.c_str());
//        auto ret = std::make_shared<TSound>();
//        ret->AudioBuffer = snd.audioBuffer;
//        ret->Buffer = snd.buffer;
//        ret->Format = snd.format;
//        ret->Source = snd.source;
//        return ret;
        return nullptr;
    }

}
