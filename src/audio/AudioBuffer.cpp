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

#include "audio/AudioBuffer.hpp"

namespace ngine::audio {
    // Public Methods
    bool AudioBuffer::isPlaying() const {
        return (Playing && !Paused);
    }

    void AudioBuffer::pause() {
        Paused = true;
    }

    void AudioBuffer::play() {
        Playing = true;
        Paused = false;
        FrameCursorPos = 0;
    }

    void AudioBuffer::resume() {
        Paused = false;
    }

    void AudioBuffer::setPitch(float pitch) {
        float pitchMul = pitch / Pitch;

        auto newOutputSampleRate = (ma_uint32)((float)DSP.src.config.sampleRateOut/pitchMul);
        Pitch *= (float)DSP.src.config.sampleRateOut/newOutputSampleRate;

        ma_pcm_converter_set_output_sample_rate(&DSP, newOutputSampleRate);
    }

    void AudioBuffer::setVolume(float vol) {
        Volume = vol;
    }

    void AudioBuffer::stop() {
        if (isPlaying()) {
            Playing = false;
            Paused = false;
            FrameCursorPos = 0;
            TotalFramesProcessed = 0;
            IsSubBufferProcessed[0] = true;
            IsSubBufferProcessed[1] = true;
        }
    }
}