#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include "../ngine.h"

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
