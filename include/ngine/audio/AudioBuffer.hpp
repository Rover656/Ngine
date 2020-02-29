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

#include "../../../vendor/miniaudio/miniaudio.h"

namespace ngine::audio {
    /**
     * Audio buffer usage type
     */
    enum class AudioBufferUsage {
        /**
         * Static buffer.
         */
        Static,

        /**
         * Streamed buffer
         */
        Stream
    };

    /**
     * An audio data buffer.
     */
    struct NEAPI AudioBuffer {
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
        AudioBufferUsage Usage;

        /**
         * Buffer volume.
         */
        float Volume;

        /**
         * Is buffer playing.
         *
         * @return Whether the buffer is playing or not.
         */
        bool isPlaying() const;

        /**
         * Pause buffer.
         */
        void pause();

        /**
         * Play buffer.
         */
        void play();

        /**
         * Resume buffer.
         */
        void resume();

        /**
         * Set buffer pitch
         *
         * @param pitch The pitch for the buffer.
         */
        void setPitch(float pitch);

        /**
         * Set buffer volume
         *
         * @param vol The volume for the buffer.
         */
        void setVolume(float vol);

        /**
         * Stop buffer
         */
        void stop();
    };
}

#endif //AUDIOBUFFER_HPP
