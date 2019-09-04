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

#ifndef MUSIC_H
#define MUSIC_H

#include "../ngine.h"

#include "../Filesystem/Filesystem.h"

namespace NerdThings::Ngine::Audio {
    /*
     * Music stream
     */
    struct NEAPI TMusic {
        // Public Fields

        /*
         * Pointer to raylib music data
         */
        void *MusicData;

        // Public Constructor(s)

        TMusic()
            : MusicData(nullptr) {}

        // Destructor

        ~TMusic();

        // Public Methods

        /*
         * Load music from a file
         */
        static std::shared_ptr<TMusic> LoadMusic(const Filesystem::TPath &path_);

        /*
         * Update music stream.
         * Should be called by AudioManager
         */
        void Update();
    };
}

#endif //MUSIC_H
