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

#include "Music.h"

namespace NerdThings::Ngine::Audio {
    // Destructor

    TMusic::~TMusic() {
//        if (MusicData != nullptr) {
//            UnloadMusicStream(static_cast<::MusicData*>(MusicData));
//            MusicData = nullptr;
//        }
    }

    // Public Methods

    std::shared_ptr<TMusic> TMusic::LoadMusic(const Filesystem::TPath &path_) {
        //auto dat = LoadMusicStream(filename_.c_str());
        //auto ret = std::make_shared<TMusic>();
        //ret->MusicData = static_cast<void*>(dat);
        //return ret;
        return nullptr;
    }

    void TMusic::Update() {
        //UpdateMusicStream(ToRaylibMusic());
    }
}
