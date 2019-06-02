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

#ifndef SOUND_H
#define SOUND_H

#include "../ngine.h"

namespace NerdThings::Ngine::Audio {
    /*
     * A sound source
     */
    struct NEAPI TSound {
        // Public Fields

        /*
         * Pointer to internal data within audio system
         */
        void *AudioBuffer;

        /*
         * Audio buffer id
         */
        unsigned int Buffer;

        /*
         * Audio format
         */
        int Format;

        /*
         * Audio source id
         */
        unsigned int Source;

        // Public Constructor(s)

        /*
         * Create a null sound
         */
        TSound()
            : AudioBuffer(nullptr), Buffer(0), Format(0), Source(0) {}

        // Destructor

        ~TSound();

        // Public Methods

        #ifdef INCLUDE_RAYLIB

        /*
         * To a raylib sound
         */
        Sound ToRaylibSound() const;

        /*
         * From a raylib sound
         */
        static TSound FromRaylibSound(Sound snd_);

        #endif

        /*
         * Load a sound from a file
         */
        static TSound *LoadSound(const std::string &filename_);
    };
}

#endif //SOUND_H
