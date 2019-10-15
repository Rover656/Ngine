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

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include "../Ngine.h"

#include "AudioBuffer.h"

namespace NerdThings::Ngine::Audio {
    /*
     * Audio stream
     */
    struct AudioStream {
        // Public Fields

        /*
         * The audio buffer
         */
        AudioBuffer *Buffer;

        /*
         * Number of channels
         */
        unsigned int Channels;

        /*
         * Sample rate
         */
        unsigned int SampleRate;

        /*
         * Bit depth
         */
        unsigned int SampleSize;

        // Public Methods

        /*
         * Update stream buffers with data
         */
        void UpdateStream(const void* data_, int samplesCount_);
    };
}

#endif //AUDIOSTREAM_H
