/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef MUSIC_HPP
#define MUSIC_HPP

#include "../Config.hpp"

#include "../Filesystem/Filesystem.hpp"
#include "../Resource.hpp"
#include "AudioStream.hpp"

namespace NerdThings::Ngine::Audio {
    /**
     * The music context type.
     */
    enum MusicContextType {
        /**
         * OGG Music context.
         */
        AUDIO_OGG = 1,

        /**
         * FLAC Music context.
         */
        AUDIO_FLAC,

        /**
         * MP3 Music context.
         */
        AUDIO_MP3,

        /**
         * XM Music context.
         *
         * @note Not implemented.
         */
        MODULE_XM,

        /**
         * MOD Music context.
         *
         * @note Not implemented.
         */
        MODULE_MOD
    };

    /**
     * Music stream
     */
    struct NEAPI Music : public IResource {
        /**
         * Context data.
         */
        void *CTXData = nullptr;

        /**
         * Music context type.
         */
        MusicContextType CTXType;

        /**
         * Number of times to loop.
         *
         * @note Set to 0 for infinite.
         */
        unsigned int LoopCount;

        /**
         * Total number of samples.
         */
        unsigned int SampleCount;

        /**
         * The audio stream.
         */
        AudioStream Stream;

        ~Music();

        /**
         * Get length in seconds.
         *
         * @return Music length in seconds.
         */
        float GetLength();

        /**
         * Get the current amount of time played in seconds.
         *
         * @return The amount of time the music has played through.
         */
        float GetTimePlayed();

        /**
         * Whether or not music is playing.
         *
         * @return Whether or not the music is playing.
         */
        bool IsPlaying() const;

        /**
         * Test if the music is valid.
         *
         * @return Whether the music is valid or not.
         */
        bool IsValid() const override;

        /**
         * Load music from a file.
         *
         * @param path_ The music file to be loaded.
         * @return The Music.
         */
        static Music *LoadMusic(const Filesystem::Path &path_);

        /**
         * Pause music.
         */
        void Pause();

        /**
         * Play music.
         */
        void Play();

        /**
         * Resume music.
         */
        void Resume();

        /**
         * Set music pitch.
         *
         * @param pitch_ The pitch.
         */
        void SetPitch(float pitch_);

        /**
         * Set music volume.
         *
         * @param vol_ The music volume.
         */
        void SetVolume(float vol_);

        /**
         * Stop music.
         */
        void Stop();

        /**
         * Unload the music.
         */
        void Unload() override;

        /**
         * Update all active music.
         */
        static void Update();

    private:
        // Private Fields

        static std::vector<Music *> _ActiveMusic;

        /*
         * Number of loops completed since last play.
         */
        unsigned int _LoopsCompleted;

        // Private Methods

        /*
         * Update music stream.
         */
        void __Update();
    };
}

#endif //MUSIC_HPP
