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
