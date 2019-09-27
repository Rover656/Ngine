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
    struct NEAPI Music : public IResource {
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
        AudioStream Stream;

        // Destructor

        ~Music();

        // Public Methods

        /*
         * Get length in seconds
         */
        float GetLength();

        /*
         * Get the current amount of time played in seconds.
         */
        float GetTimePlayed();

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
        static Music *LoadMusic(const Filesystem::Path &path_);

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

        static std::vector<Music *> _ActiveMusic;

        /*
         * Number of loops completed since last play.
         */
        int _LoopsCompleted;

        // Private Methods

        /*
         * Update music stream.
         */
        void __Update();
    };
}

#endif //MUSIC_H
