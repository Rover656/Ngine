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

#include "../filesystem/Filesystem.hpp"
#include "../Resource.hpp"
#include "AudioStream.hpp"

namespace ngine::audio {
    /**
     * The music context type.
     */
    enum class MusicContextType {
        /**
         * OGG Music context.
         */
        OGG = 1,

        /**
         * FLAC Music context.
         */
        FLAC,

        /**
         * MP3 Music context.
         */
        MP3,

        /**
         * XM Music context.
         *
         * @note Not implemented.
         */
        XM,

        /**
         * MOD Music context.
         *
         * @note Not implemented.
         */
        MOD
    };

    /**
     * Music stream
     */
    class NEAPI Music : public IResource {
        /**
         * List of all playing music.
         */
        static std::vector<Music *> m_activeMusic;

        /**
         * Number of loops completed since last play.
         */
        unsigned int m_loopsCompleted;

        /**
         * Update music stream.
         */
        void _update();
    public:
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
        float getLength();

        /**
         * Get the current amount of time played in seconds.
         *
         * @return The amount of time the music has played through.
         */
        float getTimePlayed();

        /**
         * Whether or not music is playing.
         *
         * @return Whether or not the music is playing.
         */
        bool isPlaying() const;

        /**
         * Test if the music is valid.
         *
         * @return Whether the music is valid or not.
         */
        bool isValid() const override;

        /**
         * Load music from a file.
         *
         * @param path_ The music file to be loaded.
         * @return The Music.
         */
        static Music *LoadMusic(const filesystem::Path &path_);

        /**
         * pause music.
         */
        void pause();

        /**
         * Play music.
         */
        void play();

        /**
         * Resume music.
         */
        void resume();

        /**
         * Set music pitch.
         *
         * @param pitch_ The pitch.
         */
        void setPitch(float pitch_);

        /**
         * Set music volume.
         *
         * @param vol_ The music volume.
         */
        void setVolume(float vol_);

        /**
         * Stop music.
         */
        void stop();

        /**
         * Free the music.
         */
        void free() override;

        /**
         * Update all active music.
         */
        static void Update();
    };
}

#endif //MUSIC_HPP
