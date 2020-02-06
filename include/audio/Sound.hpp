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

#ifndef SOUND_HPP
#define SOUND_HPP

#include "../Config.hpp"

#include "../filesystem/Filesystem.hpp"
#include "../Resource.hpp"
#include "AudioStream.hpp"
#include "Wave.hpp"

namespace ngine::audio {
    /**
     * A sound.
     */
    class NEAPI Sound : public IResource {
    public:
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
        Sound() = default;
        ~Sound();

        /**
         * Whether or not the sound is playing.
         *
         * @return Whether the sound is playing or not.
         */
        bool isPlaying() const;

        /**
         * Whether or not the sound is valid.
         *
         * @return Whether the sound is valid or not.
         */
        bool isValid() const override;

        /**
         * Load a sound from a file.
         *
         * @param path_ The file to load sound from.
         * @return The loaded sound.
         */
        static Sound *LoadSound(const filesystem::Path &path_);

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
        void pause();

        /**
         * Play sound.
         */
        void play();

        /**
         * Resume sound.
         */
        void resume();

        /**
         * Set sound pitch.
         *
         * @param pitch_ Sound pitch.
         */
        void setPitch(float pitch_);

        /**
         * Set sound volume.
         *
         * @param vol_ Sound volume.
         */
        void setVolume(float vol_);

        /**
         * Stop sound.
         */
        void stop();

        /**
         * Free sound.
         */
        void free() override;
    };
}

#endif //SOUND_HPP