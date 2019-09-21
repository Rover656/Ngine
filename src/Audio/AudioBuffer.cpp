#include "AudioBuffer.h"

namespace NerdThings::Ngine::Audio {
    // Public Methods
    bool TAudioBuffer::IsPlaying() const {
        return (Playing && !Paused);
    }

    void TAudioBuffer::Pause() {
        Paused = true;
    }

    void TAudioBuffer::Play() {
        Playing = true;
        Paused = false;
        FrameCursorPos = 0;
    }

    void TAudioBuffer::Resume() {
        Paused = false;
    }

    void TAudioBuffer::SetPitch(float pitch_) {
        float pitchMul = pitch_ / Pitch;

        auto newOutputSampleRate = (ma_uint32)((float)DSP.src.config.sampleRateOut/pitchMul);
        Pitch *= (float)DSP.src.config.sampleRateOut/newOutputSampleRate;

        ma_pcm_converter_set_output_sample_rate(&DSP, newOutputSampleRate);
    }

    void TAudioBuffer::SetVolume(float vol_) {
        Volume = vol_;
    }

    void TAudioBuffer::Stop() {
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