/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
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
