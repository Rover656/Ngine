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

#include "ngine/audio/Sound.hpp"

#include <miniaudio.h>

#include "ngine/Console.hpp"
#include "ngine/audio/AudioDevice.hpp"

namespace ngine::audio {
    // Destructor

    Sound::~Sound() { free(); }

    // Public Methods

    bool Sound::isPlaying() const { return Stream.Buffer->isPlaying(); }

    bool Sound::isValid() const { return Stream.Buffer != nullptr; }

    Sound *Sound::loadSound(const filesystem::Path &path) {
        auto wav = Wave::loadWave(path);

        auto snd = loadSoundFromWave(wav);

        delete wav;

        return snd;
    }

    Sound *Sound::loadSoundFromWave(Wave *wave) {
        if (wave->isValid()) {
            auto snd = new Sound();

            // We convert every sound to be the same format.
            ma_format formatIn =
                ((wave->SampleSize == 8)
                     ? ma_format_u8
                     : ((wave->SampleSize == 16) ? ma_format_s16
                                                 : ma_format_f32));
            ma_uint32 frameCountIn = wave->SampleCount / wave->Channels;

            ma_uint32 frameCount = (ma_uint32)ma_convert_frames(
                nullptr, DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE,
                nullptr, formatIn, wave->Channels, wave->SampleRate,
                frameCountIn);
            if (frameCount == 0)
                Console::warn(
                    "Sound",
                    "Failed to get frame count for format conversion!");

            auto audioBuffer = AudioDevice::initAudioBuffer(
                DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE, frameCount,
                AudioBufferUsage::Static);
            if (audioBuffer == nullptr)
                Console::warn("Sound", "Failed to create audio buffer!");

            frameCount = (ma_uint32)ma_convert_frames(
                audioBuffer->Buffer,
                audioBuffer->DSP.formatConverterIn.config.formatIn,
                audioBuffer->DSP.formatConverterIn.config.channels,
                audioBuffer->DSP.src.config.sampleRateIn, wave->Data, formatIn,
                wave->Channels, wave->SampleRate, frameCountIn);
            if (frameCount == 0)
                Console::warn("Sound", "Format conversion failed!");

            snd->SampleCount = frameCount * DEVICE_CHANNELS;
            snd->Stream.SampleRate = DEVICE_SAMPLE_RATE;
            snd->Stream.SampleSize = 32;
            snd->Stream.Channels = DEVICE_CHANNELS;
            snd->Stream.Buffer = audioBuffer;

            return snd;
        }

        return nullptr;
    }

    void Sound::pause() { Stream.Buffer->pause(); }

    void Sound::play() { Stream.Buffer->play(); }

    void Sound::resume() { Stream.Buffer->resume(); }

    void Sound::setPitch(float pitch) { Stream.Buffer->setPitch(pitch); }

    void Sound::setVolume(float vol) { Stream.Buffer->setVolume(vol); }

    void Sound::stop() { Stream.Buffer->stop(); }

    void Sound::free() {
        // Close buffer
        AudioDevice::closeAudioBuffer(Stream.Buffer);
        Stream.Buffer = nullptr;
    }
} // namespace ngine::audio
