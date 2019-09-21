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
#include "../Resource.h"
#include "AudioStream.h"

namespace NerdThings::Ngine::Audio {
    enum EMusicContextType {
        AUDIO_OGG = 1,
        AUDIO_FLAC,
        AUDIO_MP3,
        MODULE_XM,
        MODULE_MOD
    };

    /*
     * Music stream
     */
    struct NEAPI TMusic : public TResource {
        // Public Fields

        /*
         * Context data
         */
        void *CTXData = nullptr;

        /*
         * Music context type
         */
        EMusicContextType CTXType;

        /*
         * Number of times to loop, 0 = infinite.
         */
        unsigned int LoopCount;

        /*
         * Total number of samples
         */
        unsigned int SampleCount;

        /*
         * The audio stream
         */
        TAudioStream Stream;

        // Destructor

        ~TMusic();

        // Public Methods

        /*
         * Whether or not music is playing
         */
        bool IsPlaying() const;

        /*
         * Test if the music is valid
         */
        bool IsValid() const override;

        /*
         * Load music from a file
         */
        static TMusic *LoadMusic(const Filesystem::TPath &path_);

        /*
         * Pause music
         */
        void Pause();

        /*
         * Play music
         */
        void Play();

        /*
         * Resume music
         */
        void Resume();

        /*
         * Set music pitch
         */
        void SetPitch(float pitch_);

        /*
         * Set music volume
         */
        void SetVolume(float vol_);

        /*
         * Stop music
         */
        void Stop();

        /*
         * Unload the music
         */
        void Unload() override;

        /*
         * Update all active music
         */
        static void Update();

    private:
        // Private Fields

        static std::vector<TMusic *> _ActiveMusic;

        // Private Methods

        /*
         * Update music stream.
         */
        void __Update();
    };
}

#endif //MUSIC_H
