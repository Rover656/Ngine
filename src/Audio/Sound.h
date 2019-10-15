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

#include "../Ngine.h"

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"
#include "AudioStream.h"
#include "Wave.h"

namespace NerdThings::Ngine::Audio {
    /*
     * A sound source
     */
    struct NEAPI Sound : public IResource {
        // Public Fields

        /*
         * The number of samples
         */
        unsigned int SampleCount;

        /*
         * The audio stream
         */
        AudioStream Stream;

        // Public Constructor(s)

        /*
         * Create a null sound
         */
        Sound() {}

        // Destructor

        ~Sound();

        // Public Methods

        /*
         * Whether or not the sound is playing.
         */
        bool IsPlaying() const;

        /*
         * Whether or not the sound is valid
         */
        bool IsValid() const override;

        /*
         * Load a sound from a file
         */
        static Sound *LoadSound(const Filesystem::Path &path_);

        /*
         * Load sound from wave data.
         */
        static Sound *LoadSoundFromWave(Wave *wave_);

        /*
         * Pause sound
         */
        void Pause();

        /*
         * Play sound
         */
        void Play();

        /*
         * Resume sound
         */
        void Resume();

        /*
         * Set sound pitch
         */
        void SetPitch(float pitch_);

        /*
         * Set sound volume
         */
        void SetVolume(float vol_);

        /*
         * Stop sound
         */
        void Stop();

        /*
         * Unload sound
         */
        void Unload() override;
    };
}

#endif //SOUND_H
