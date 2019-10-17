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

#include "AudioBuffer.h"

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