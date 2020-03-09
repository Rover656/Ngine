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

#ifndef WAVE_HPP
#define WAVE_HPP

#include "../Config.hpp"

#include "../Resource.hpp"
#include "../filesystem/Filesystem.hpp"

namespace ngine::audio {
    /**
     * Wave sound data.
     */
    class NEAPI Wave : public IResource {
        void _loadFLAC(const filesystem::Path &path);
        void _loadMP3(const filesystem::Path &path);
        void _loadOGG(const filesystem::Path &path);
        void _loadWAV(const filesystem::Path &path);

    public:
        /**
         * Number of audio channels.
         */
        unsigned int Channels;

        /**
         * Audio data.
         */
        void *Data;

        /**
         * Number of samples.
         */
        unsigned int SampleCount;

        /**
         * Sample rate.
         */
        unsigned int SampleRate;

        /**
         * Sample size.
         */
        unsigned int SampleSize;

        ~Wave();

        /**
         * Is wave sound valid?
         *
         * @return Whether or not the sound is valid.
         */
        bool isValid() const override;

        /**
         * Load wave file.
         *
         * @param path The file to load wave data from.
         * @return The wave data.
         */
        static Wave *loadWave(const filesystem::Path &path);

        /**
         * Free wave file.
         */
        void free() override;
    };
} // namespace ngine::audio

#endif // WAVE_HPP
