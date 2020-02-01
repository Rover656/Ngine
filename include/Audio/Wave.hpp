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

#include "../Filesystem/Filesystem.hpp"
#include "../Resource.hpp"

namespace Ngine::Audio {
    /**
     * Wave sound data.
     */
    struct NEAPI Wave : public IResource {
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
        bool IsValid() const override;

        /**
         * Load wave file.
         *
         * @param path_ The file to load wave data from.
         * @return The wave data.
         */
        static Wave *LoadWave(const Filesystem::Path &path_);

        /**
         * Free wave file.
         */
        void Free() override;

    private:
        void __LoadFLAC(const Filesystem::Path &path_);
        void __LoadMP3(const Filesystem::Path &path_);
        void __LoadOGG(const Filesystem::Path &path_);
        void __LoadWAV(const Filesystem::Path &path_);
    };
}

#endif //WAVE_HPP
