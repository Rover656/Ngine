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
        if (MusicData != nullptr) {
            UnloadMusicStream(static_cast<::MusicData*>(MusicData));
            MusicData = nullptr;
        }
    }

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Music TMusic::ToRaylibMusic() const {
        return static_cast<::MusicData*>(MusicData);
    }

    TMusic TMusic::FromRaylibMusic(Music music_) {
        auto mus = TMusic();
        mus.MusicData = static_cast<void*>(music_);
        return mus;
    }

    #endif

    TMusic *TMusic::LoadMusic(const std::string &filename_) {
        auto dat = LoadMusicStream(filename_.c_str());
        auto ret = new TMusic;
        ret->MusicData = static_cast<void*>(dat);
        return ret;
    }

    void TMusic::Update() {
        UpdateMusicStream(ToRaylibMusic());
    }
}
