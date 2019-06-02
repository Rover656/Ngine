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

        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to raylib music
         */
        Music ToRaylibMusic() const;

        /*
         * Convert from raylib music
         */
        static TMusic FromRaylibMusic(Music music_);

        #endif

        /*
         * Load music from a file
         */
        static TMusic *LoadMusic(const std::string &filename_);

        /*
         * Update music stream.
         * Should be called by AudioManager
         */
        void Update();
    };
}

#endif //MUSIC_H
