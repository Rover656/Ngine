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

#include "Audio/AudioDevice.hpp"
#include "Console.hpp"

#include <cstring>

namespace ngine::audio {
    std::vector<Music *> AudioDevice::m_activeMusic;

    ma_mutex AudioDevice::m_audioLock;
    AudioBuffer *AudioDevice::m_bufferFirst;
    AudioBuffer *AudioDevice::m_bufferLast;
    ma_context AudioDevice::m_context;
    ma_device AudioDevice::m_device;
    bool AudioDevice::m_initialized = false;
    float AudioDevice::m_masterVolume = 1.0f;

    ma_uint32 AudioDevice::_audioBufferDSPRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount,
                                               void *pUserData) {
        if (!m_initialized) return 0;
        auto buffer = (AudioBuffer *)pUserData;

        auto subBufferSizeInFrames = (buffer->BufferSizeInFrames > 1) ? buffer->BufferSizeInFrames/2 : buffer->BufferSizeInFrames;
        auto currentSubBufferIndex = buffer->FrameCursorPos/subBufferSizeInFrames;

        if (currentSubBufferIndex > 1) {
            Console::Warn("AudioDevice", "Frame cursor position moved too far forward in audio stream.");
            return 0;
        }

        // Another thread can update the processed state of buffers so save a copy to stop sync issues
        bool isSubBufferProcessed[2];
        isSubBufferProcessed[0] = buffer->IsSubBufferProcessed[0];
        isSubBufferProcessed[1] = buffer->IsSubBufferProcessed[1];

        auto frameSizeInBytes = ma_get_bytes_per_sample(buffer->DSP.formatConverterIn.config.formatIn)*buffer->DSP.formatConverterIn.config.channels;

        // Fill every frame until we find a buffer marked as processed.
        ma_uint32 framesRead = 0;
        while(true) {
            if (buffer->Usage == AudioBufferUsage::Static) {
                if (framesRead >= frameCount) break;
            } else if (isSubBufferProcessed[currentSubBufferIndex]) break;

            ma_uint32 totalFramesRemaining = frameCount - framesRead;
            if (totalFramesRemaining == 0) break;

            ma_uint32 framesRemainingInOutputBuffer;
            if (buffer->Usage == AudioBufferUsage::Stream) {
                framesRemainingInOutputBuffer = buffer->BufferSizeInFrames - buffer->FrameCursorPos;
            } else {
                ma_uint32 firstFrameIndexOfThisSubBuffer = subBufferSizeInFrames*currentSubBufferIndex;
                framesRemainingInOutputBuffer = subBufferSizeInFrames - (buffer->FrameCursorPos - firstFrameIndexOfThisSubBuffer);
            }

            ma_uint32 framesToRead = totalFramesRemaining;
            if (framesToRead > framesRemainingInOutputBuffer) framesToRead = framesRemainingInOutputBuffer;

            memcpy((unsigned char *)pFramesOut + (framesRead*frameSizeInBytes), (unsigned char *)buffer->Buffer + (buffer->FrameCursorPos*frameSizeInBytes), framesToRead*frameSizeInBytes);
            buffer->FrameCursorPos = (buffer->FrameCursorPos + framesToRead)%buffer->BufferSizeInFrames;
            framesRead += framesToRead;

            // mark as processed if we reach end
            if (framesToRead == framesRemainingInOutputBuffer) {
                buffer->IsSubBufferProcessed[currentSubBufferIndex] = true;
                isSubBufferProcessed[currentSubBufferIndex] = true;

                currentSubBufferIndex = (currentSubBufferIndex + 1)%2;

                // Stop if not looping
                if (!buffer->Looping) {
                    buffer->stop();
                    break;
                }
            }
        }

        // Zero-fill excess
        ma_uint32 totalFramesRemaining = (frameCount - framesRead);
        if (totalFramesRemaining > 0) {
            memset((unsigned char *)pFramesOut + (framesRead*frameSizeInBytes), 0, totalFramesRemaining*frameSizeInBytes);

            // For static buffers, fill with silence for safety but dont report as read
            if (buffer->Usage != AudioBufferUsage::Static) framesRead += totalFramesRemaining;
        }

        return framesRead;
    }

    void AudioDevice::_logCallback(ma_context *pContext, ma_device *pDevice, ma_uint32 logLevel, const char *msg) {
        Console::Error("AudioDevice - miniaudio", std::string(msg));
    }

    void AudioDevice::_mixAudioFrames(float *framesOut_, const float *framesIn_, ma_uint32 frameCount_,
                                      float localVolume_) {
        for (ma_uint32 iFrame = 0; iFrame < frameCount_; ++iFrame) {
            for (ma_uint32 iChannel = 0; iChannel < m_device.playback.channels; ++iChannel) {
                float *frameOut = framesOut_ + (iFrame * m_device.playback.channels);
                const float *frameIn = framesIn_ + (iFrame * m_device.playback.channels);

                frameOut[iChannel] += (frameIn[iChannel] * m_masterVolume * localVolume_);
            }
        }
    }

    void AudioDevice::_sendAudioDataToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput,
                                             ma_uint32 frameCount) {
        if (!m_initialized) return;
        // Mixing is just an accumulation, init output buffer to 0
        memset(pFramesOut, 0, frameCount*pDevice->playback.channels*ma_get_bytes_per_sample(pDevice->playback.format));

        // Mutex for thread safety
        ma_mutex_lock(&m_audioLock);
        {
            for (auto buffer = m_bufferFirst; buffer != nullptr; buffer = buffer->Next) {
                // Ignore stopped or paused sounds
                if (!buffer->Playing || buffer->Paused) continue;

                ma_uint32 framesRead = 0;

                while(true) {
                    if (framesRead > frameCount) {
                        Console::Warn("AudioDevice", "Mixed too many frames from audio buffer.");
                        break;
                    }

                    if (framesRead == frameCount) break;

                    // Read as much as we can
                    ma_uint32 framesToRead = (frameCount - framesRead);

                    while (framesToRead > 0) {
                        float tmpBuffer[1024];

                        ma_uint32 framesToReadRightNow = framesToRead;
                        if (framesToReadRightNow > sizeof(tmpBuffer)/sizeof(tmpBuffer[0])/DEVICE_CHANNELS) {
                            framesToReadRightNow = sizeof(tmpBuffer)/sizeof(tmpBuffer[0])/DEVICE_CHANNELS;
                        }

                        ma_uint32 framesJustRead = (ma_uint32)ma_pcm_converter_read(&buffer->DSP, tmpBuffer, framesToReadRightNow);
                        if (framesJustRead > 0) {
                            float *framesOut = (float *)pFramesOut + (framesRead * m_device.playback.channels);
                            float *framesIn = tmpBuffer;

                            _mixAudioFrames(framesOut, framesIn, framesJustRead, buffer->Volume);

                            framesToRead -= framesJustRead;
                            framesRead += framesJustRead;
                        }

                        if (framesJustRead < framesToReadRightNow) {
                            if (!buffer->Looping) {
                                buffer->stop();
                                break;
                            } else {
                                buffer->FrameCursorPos = 0;
                                continue;
                            }
                        }
                    }

                    if (framesToRead > 0) break;
                }
            }
        }
        ma_mutex_unlock(&m_audioLock);
    }

    void AudioDevice::_trackAudioBuffer(AudioBuffer *buffer_) {
        if (!m_initialized) return;
        ma_mutex_lock(&m_audioLock);
        {
            if (m_bufferFirst == nullptr) m_bufferFirst = buffer_;
            else {
                m_bufferLast->Next = buffer_;
                buffer_->Prev = m_bufferLast;
            }

            m_bufferLast = buffer_;
        }
        ma_mutex_unlock(&m_audioLock);
    }

    void AudioDevice::_untrackAudioBuffer(AudioBuffer *buffer_) {
        if (!m_initialized) return;
        ma_mutex_lock(&m_audioLock);
        {
            if (buffer_->Prev == nullptr) m_bufferFirst = buffer_->Next;
            else buffer_->Prev->Next = buffer_->Next;

            if (buffer_->Next == nullptr) m_bufferLast = buffer_->Prev;
            else buffer_->Next->Prev = buffer_->Prev;

            buffer_->Prev = nullptr;
            buffer_->Next = nullptr;
        }
        ma_mutex_unlock(&m_audioLock);
    }

    void AudioDevice::CloseAudioBuffer(AudioBuffer *buffer_) {
        if (!m_initialized) return;
        if (buffer_ != nullptr) {
            // Untrack buffer
            _untrackAudioBuffer(buffer_);

            // Delete
            free(buffer_->Buffer);
            delete buffer_;
        }
    }

    void AudioDevice::Close() {
        if (m_initialized) {
            m_initialized = false;

            ma_mutex_uninit(&m_audioLock);
            ma_device_uninit(&m_device);
            ma_context_uninit(&m_context);

            Console::Notice("AudioDevice", "Audio device closed successfully.");

        } else Console::Warn("AudioDevice", "Could not close audio device as it is not open.");
    }

    AudioBuffer *AudioDevice::InitAudioBuffer(ma_format format_, ma_uint32 channels_, ma_uint32 sampleRate_,
                                              ma_uint32 bufferSizeInFrames_, AudioBufferUsage usage_) {
        if (!m_initialized) return nullptr;
        auto *buffer = new AudioBuffer();

        buffer->Buffer = calloc(bufferSizeInFrames_ * channels_ * ma_get_bytes_per_sample(format_), 1);

        if (buffer == nullptr || buffer->Buffer == nullptr) {
            Console::Error("AudioDevice", "Failed to allocate memory for audio buffer.");
            free(buffer->Buffer);
            delete buffer;
            return nullptr;
        }

        // Audio data passes through a format converted
        ma_pcm_converter_config dspConfig;
        memset(&dspConfig, 0, sizeof(dspConfig));
        dspConfig.formatIn = format_;
        dspConfig.formatOut = DEVICE_FORMAT;
        dspConfig.channelsIn = channels_;
        dspConfig.channelsOut = DEVICE_CHANNELS;
        dspConfig.sampleRateIn = sampleRate_;
        dspConfig.sampleRateOut = DEVICE_SAMPLE_RATE;
        dspConfig.onRead = _audioBufferDSPRead;
        dspConfig.pUserData = buffer;
        dspConfig.allowDynamicSampleRate = true;

        ma_result result = ma_pcm_converter_init(&dspConfig, &buffer->DSP);
        if (result != MA_SUCCESS) {
            Console::Error("AudioDevice", "Failed to create data conversion pipeline.");
            free(buffer->Buffer);
            delete buffer;
            return nullptr;
        }

        // Init buffer values
        buffer->Volume = 1.0f;
        buffer->Pitch = 1.0f;
        buffer->Playing = false;
        buffer->Paused = false;
        buffer->Looping = false;
        buffer->Usage = usage_;
        buffer->FrameCursorPos = 0;
        buffer->BufferSizeInFrames = bufferSizeInFrames_;

        // Buffers should be marked as processed bu default so they call UpdateAudioStream
        buffer->IsSubBufferProcessed[0] = true;
        buffer->IsSubBufferProcessed[1] = true;

        // Track buffer
        _trackAudioBuffer(buffer);

        return buffer;
    }

    AudioStream
    AudioDevice::InitAudioStream(unsigned int sampleRate_, unsigned int sampleSize_, unsigned int channels_) {
        AudioStream stream;

        stream.SampleRate = sampleRate_;
        stream.SampleSize = sampleSize_;
        stream.Channels = channels_;

        ma_format formatIn = ((stream.SampleSize == 8) ? ma_format_u8 : ((stream.SampleSize == 16) ? ma_format_s16 : ma_format_f32));

        // The size of a streaming buffer must be at least double the size of a period
        unsigned int periodSize = m_device.playback.internalBufferSizeInFrames / m_device.playback.internalPeriods;
        unsigned int subBufferSize = AUDIO_BUFFER_SIZE;

        if (subBufferSize < periodSize) subBufferSize = periodSize;

        stream.Buffer = InitAudioBuffer(formatIn, stream.Channels, stream.SampleRate, subBufferSize*2, AudioBufferUsage::Stream);
        
        if (stream.Buffer != nullptr) {
            stream.Buffer->Looping = true; // Always loop streams
            Console::Notice("AudioDevice", "Audio stream loaded successfully.");
        } else Console::Error("AudioDevice", "Failed to create audio buffer.");

        return stream;
    }

    void AudioDevice::Initialize() {
        // Cant run twice
        if (m_initialized) return;

        // Init context
        ma_context_config contextConfig = ma_context_config_init();
        contextConfig.logCallback = _logCallback;

        ma_result result = ma_context_init(nullptr, 0, &contextConfig, &m_context);
        if (result != MA_SUCCESS) {
            Console::Error("AudioDevice", "Failed to initialize audio context.");
            return;
        }

        // Init audio device
        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.pDeviceID = nullptr; // Default
        config.playback.format = DEVICE_FORMAT;
        config.playback.channels = DEVICE_CHANNELS;
        config.capture.pDeviceID = nullptr;
        config.capture.format = ma_format_s16;
        config.capture.channels = 1;
        config.sampleRate = DEVICE_SAMPLE_RATE;
        config.dataCallback = _sendAudioDataToDevice;
        config.pUserData = nullptr;

        result = ma_device_init(&m_context, &config, &m_device);
        if (result != MA_SUCCESS) {
            Console::Error("AudioDevice", "Failed to initialize audio playback device.");
            ma_context_uninit(&m_context);
            return;
        }

        // Keep device running all the time.
        result = ma_device_start(&m_device);
        if (result != MA_SUCCESS) {
            Console::Error("AudioDevice", "Failed to start audio playback device.");
            ma_device_uninit(&m_device);
            ma_context_uninit(&m_context);
            return;
        }

        // Thread mixer
        result = ma_mutex_init(&m_context, &m_audioLock);
        if (result != MA_SUCCESS) {
            Console::Error("AudioDevice", "Failed to create mutex for audio mixing.");
            ma_device_uninit(&m_device);
            ma_context_uninit(&m_context);
            return;
        }

        Console::Notice("AudioDevice", "Audio device initialized successfully!");
        Console::Notice("AudioDevice", "Audio backend: miniaudio/%s", ma_get_backend_name(m_context.backend));
        Console::Notice("AudioDevice", "Audio format: %s -> %s", ma_get_format_name(m_device.playback.format), ma_get_format_name(m_device.playback.internalFormat));

        m_initialized = true;
    }

    bool AudioDevice::IsReady() {
        return m_initialized;
    }

    void AudioDevice::SetMasterVolume(float vol_) {
        m_masterVolume = vol_;
        if (m_masterVolume < 0.0f) m_masterVolume = 0.0f;
        if (m_masterVolume > 1.0f) m_masterVolume = 1.0f;
    }

    void AudioDevice::Update() {
        // We update here so we can add any future stuff
        Music::Update();
    }
}
