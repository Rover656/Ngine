#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include "../ngine.h"

#include "../../third-party/miniaudio/miniaudio.h"

namespace NerdThings::Ngine::Audio {
    enum EAudioBufferUsage {
        BUFFER_USAGE_STATIC = 0,
        BUFFER_USAGE_STREAM
    };

    struct NEAPI AudioBuffer {
        // Public Fields

        /*
         * Data buffer
         */
        void *Buffer;

        /*
         * Total buffer size in frames
         */
        unsigned int BufferSizeInFrames;

        /*
         * PCM Data converter
         */
        ma_pcm_converter DSP;

        /*
         * Frame cursor position
         */
        unsigned int FrameCursorPos;

        /*
         * Sub buffer processed (virtual double buffer)
         */
        bool IsSubBufferProcessed[2];

        /*
         * Whether or not the buffer is looping
         */
        bool Looping;

        /*
         * The next buffer in the list
         */
        AudioBuffer *Next;

        /*
         * Whether or not the buffer is paused
         */
        bool Paused;

        /*
         * Buffer pitch
         */
        float Pitch;

        /*
         * Whether or not the buffer is playing
         */
        bool Playing;

        /*
         * Previous buffer on the list
         */
        AudioBuffer *Prev;

        /*
         * Total frames processed in this buffer
         */
        unsigned int TotalFramesProcessed;

        /*
         * Audio usage mode (STATIC/STREAM)
         */
        int Usage;

        /*
         * Buffer volume
         */
        float Volume;

        // Public Methods

        /*
         * Is buffer playing
         */
        bool IsPlaying() const;

        /*
         * Pause buffer
         */
        void Pause();

        /*
         * Play buffer
         */
        void Play();

        /*
         * Resume buffer
         */
        void Resume();

        /*
         * Set buffer pitch
         */
        void SetPitch(float pitch_);

        /*
         * Set buffer volume
         */
        void SetVolume(float vol_);

        /*
         * Stop buffer
         */
        void Stop();
    };
}

#endif //AUDIOBUFFER_H
