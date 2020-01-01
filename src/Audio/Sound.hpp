/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef SOUND_HPP
#define SOUND_HPP

#include "../Config.hpp"

#include "../Filesystem/Filesystem.hpp"
#include "../Resource.hpp"
#include "AudioStream.hpp"
#include "Wave.hpp"

namespace NerdThings::Ngine::Audio {
    /**
     * A sound.
     */
    struct NEAPI Sound : public IResource {
        /**
         * The number of samples.
         */
        unsigned int SampleCount;

        /**
         * The audio stream.
         */
        AudioStream Stream;

        /**
         * Create an empty sound.
         */
        Sound() {}
        ~Sound();

        /**
         * Whether or not the sound is playing.
         *
         * @return Whether the sound is playing or not.
         */
        bool IsPlaying() const;

        /**
         * Whether or not the sound is valid.
         *
         * @return Whether the sound is valid or not.
         */
        bool IsValid() const override;

        /**
         * Load a sound from a file.
         *
         * @param path_ The file to load sound from.
         * @return The loaded sound.
         */
        static Sound *LoadSound(const Filesystem::Path &path_);

        /**
         * Load sound from wave data.
         *
         * @param wave_ Wave data to load sound with.
         * @return The loaded sound.
         */
        static Sound *LoadSoundFromWave(Wave *wave_);

        /**
         * Pause sound.
         */
        void Pause();

        /**
         * Play sound.
         */
        void Play();

        /**
         * Resume sound.
         */
        void Resume();

        /**
         * Set sound pitch.
         *
         * @param pitch_ Sound pitch.
         */
        void SetPitch(float pitch_);

        /**
         * Set sound volume.
         *
         * @param vol_ Sound volume.
         */
        void SetVolume(float vol_);

        /**
         * Stop sound.
         */
        void Stop();

        /**
         * Unload sound.
         */
        void Unload() override;
    };
}

#endif //SOUND_HPP
