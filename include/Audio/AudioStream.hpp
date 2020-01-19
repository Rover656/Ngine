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

#ifndef AUDIOSTREAM_HPP
#define AUDIOSTREAM_HPP

#include "../Config.hpp"

#include "AudioBuffer.hpp"

namespace NerdThings::Ngine::Audio {
    /**
     * Audio stream.
     */
    struct AudioStream {
        /**
         * The audio buffer.
         */
        AudioBuffer *Buffer;

        /**
         * Number of channels.
         */
        unsigned int Channels;

        /**
         * Sample rate.
         */
        unsigned int SampleRate;

        /**
         * Bit depth.
         */
        unsigned int SampleSize;

        /**
         * Update stream buffers with data.
         *
         * @param data_ New buffer data.
         * @param samplesCount_ The number of samples.
         */
        void UpdateStream(const void* data_, int samplesCount_);
    };
}

#endif //AUDIOSTREAM_HPP
