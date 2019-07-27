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

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "../ngine.h"

#include "../EventHandler.h"
#include "Music.h"
#include "Sound.h"

namespace NerdThings::Ngine::Audio {
    /*
     * Audio Manager
     */
    class NEAPI AudioManager {
        // Private Fields

        /*
         * Active music streams
         */
        static std::vector<TMusic> _ActiveMusic;

        /*
         * Whether or not the device is initialized
         */
        static bool _Initialized;

    public:
        // Public Methods

        /*
         * Close audio device.
         * Usually called by the base game class, be careful with this
         */
        static void CloseDevice();

        /*
         * Get the length of a music stream
         */
        static float GetMusicLength(TMusic mus_);

        /*
         * Get the amount of time of a music stream played
         */
        static float GetMusicTimePlayed(TMusic mus_);

        /*
         * Init audio device.
         * Usually called by the game class, be careful with this
         */
        static void InitDevice();

        /*
         * Is music stream playing
         */
        static bool IsPlaying(TMusic mus_);

        /*
         * Is sound playing
         */
        static bool IsPlaying(TSound snd_);

        /*
         * Is the device ready
         */
        static bool IsReady();

        /*
         * Pause music stream
         */
        static void Pause(TMusic mus_);

        /*
         * Pause a sound
         */
        static void Pause(TSound snd_);

        /*
         * Play music stream
         */
        static void Play(TMusic mus_);

        /*
         * Play a sound
         */
        static void Play(TSound snd_);

        /*
         * Set the master volume
         */
        static void SetMasterVolume(float vol_);

        /*
         * Set music stream loop count
         */
        static void SetLoopCount(TMusic mus_, int loops_);

        /*
         * Set music stream pitch
         */
        static void SetPitch(TMusic mus_, float pitch_);

        /*
         * Set sound pitch
         */
        static void SetPitch(TSound snd_, float pitch_);

        /*
         * Set music stream volume
         */
        static void SetVolume(TMusic mus_, float vol_);

        /*
         * Set sound volume
         */
        static void SetVolume(TSound snd_, float vol_);

        /*
         * Stop a music stream
         */
        static void Stop(TMusic mus_);

        /*
         * Stop a sound
         */
        static void Stop(TSound snd_);

        /*
         * Update
         * Handles music updating
         */
        static void Update(EventArgs &e);
    };
}

#endif //AUDIOMANAGER_H
