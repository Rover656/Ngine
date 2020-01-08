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

#include "AudioBuffer.hpp"

namespace NerdThings::Ngine::Audio {
    // Public Methods
    bool AudioBuffer::IsPlaying() const {
        return (Playing && !Paused);
    }

    void AudioBuffer::Pause() {
        Paused = true;
    }

    void AudioBuffer::Play() {
        Playing = true;
        Paused = false;
        FrameCursorPos = 0;
    }

    void AudioBuffer::Resume() {
        Paused = false;
    }

    void AudioBuffer::SetPitch(float pitch_) {
        float pitchMul = pitch_ / Pitch;

        auto newOutputSampleRate = (ma_uint32)((float)DSP.src.config.sampleRateOut/pitchMul);
        Pitch *= (float)DSP.src.config.sampleRateOut/newOutputSampleRate;

        ma_pcm_converter_set_output_sample_rate(&DSP, newOutputSampleRate);
    }

    void AudioBuffer::SetVolume(float vol_) {
        Volume = vol_;
    }

    void AudioBuffer::Stop() {
        if (IsPlaying()) {
            Playing = false;
            Paused = false;
            FrameCursorPos = 0;
            TotalFramesProcessed = 0;
            IsSubBufferProcessed[0] = true;
            IsSubBufferProcessed[1] = true;
        }
    }
}