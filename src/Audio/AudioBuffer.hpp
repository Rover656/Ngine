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

#ifndef AUDIOBUFFER_HPP
#define AUDIOBUFFER_HPP

#include "../Config.hpp"

#include "../../third-party/miniaudio/miniaudio.h"

namespace NerdThings::Ngine::Audio {
    /**
     * Audio buffer usage type
     */
    enum AudioBufferUsage {
        /**
         * Static buffer.
         */
        BUFFER_USAGE_STATIC = 0,

        /**
         * Streamed buffer
         */
        BUFFER_USAGE_STREAM
    };

    /**
     * An audio data buffer.
     */
    struct NEAPI AudioBuffer {
        // Public Fields

        /**
         * Data buffer.
         */
        void *Buffer;

        /**
         * Total buffer size in frames.
         */
        unsigned int BufferSizeInFrames;

        /**
         * PCM Data converter.
         */
        ma_pcm_converter DSP;

        /**
         * Frame cursor position.
         */
        unsigned int FrameCursorPos;

        /**
         * Sub buffer processed (virtual double buffer).
         */
        bool IsSubBufferProcessed[2];

        /**
         * Whether or not the buffer is looping.
         */
        bool Looping;

        /**
         * The next buffer in the list.
         */
        AudioBuffer *Next;

        /**
         * Whether or not the buffer is paused.
         */
        bool Paused;

        /**
         * Buffer pitch
         */
        float Pitch;

        /**
         * Whether or not the buffer is playing.
         */
        bool Playing;

        /**
         * Previous buffer on the list.
         */
        AudioBuffer *Prev;

        /**
         * Total frames processed in this buffer.
         */
        unsigned int TotalFramesProcessed;

        /**
         * Audio usage mode.
         */
        int Usage;

        /**
         * Buffer volume.
         */
        float Volume;

        /**
         * Is buffer playing.
         *
         * @return Whether the buffer is playing or not.
         */
        bool IsPlaying() const;

        /**
         * Pause buffer.
         */
        void Pause();

        /**
         * Play buffer.
         */
        void Play();

        /**
         * Resume buffer.
         */
        void Resume();

        /**
         * Set buffer pitch
         *
         * @param pitch_ The pitch for the buffer.
         */
        void SetPitch(float pitch_);

        /**
         * Set buffer volume
         *
         * @param vol_ The volume for the buffer.
         */
        void SetVolume(float vol_);

        /**
         * Stop buffer
         */
        void Stop();
    };
}

#endif //AUDIOBUFFER_HPP
