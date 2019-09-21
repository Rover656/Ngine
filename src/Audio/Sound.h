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

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"
#include "AudioStream.h"
#include "Wave.h"

namespace NerdThings::Ngine::Audio {
    /*
     * A sound source
     */
    struct NEAPI TSound : public TResource {
        // Public Fields

        /*
         * The number of samples
         */
        unsigned int SampleCount;

        /*
         * The audio stream
         */
        TAudioStream Stream;

        // Public Constructor(s)

        /*
         * Create a null sound
         */
        TSound() {}

        // Destructor

        ~TSound();

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
        static TSound *LoadSound(const Filesystem::TPath &path_);

        /*
         * Load sound from wave data.
         */
        static TSound *LoadSoundFromWave(TWave *wave_);

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
