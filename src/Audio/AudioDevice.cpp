/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "AudioDevice.h"

// To start with, copying raylib so we understand how the system works
// Then going to begin adding any improvements or features we need
// Similarly like we did with the OpenGL abstraction

namespace NerdThings::Ngine::Audio {
    // Private Fields

    std::vector<TMusic *> AudioDevice::_ActiveMusic;

    ma_mutex AudioDevice::_AudioLock;
    TAudioBuffer *AudioDevice::_BufferFirst;
    TAudioBuffer *AudioDevice::_BufferLast;
    ma_context AudioDevice::_Context;
    ma_device AudioDevice::_Device;
    bool AudioDevice::_Initialized = false;
    float AudioDevice::_MasterVolume = 1.0f;

    // Private Methods

    ma_uint32 AudioDevice::__AudioBufferDSPRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount,
                                                void *pUserData) {
        if (!_Initialized) return 0;
        auto buffer = (TAudioBuffer *)pUserData;

        auto subBufferSizeInFrames = (buffer->BufferSizeInFrames > 1) ? buffer->BufferSizeInFrames/2 : buffer->BufferSizeInFrames;
        auto currentSubBufferIndex = buffer->FrameCursorPos/subBufferSizeInFrames;

        if (currentSubBufferIndex > 1) {
            ConsoleMessage("Frame cursor position moved too far forward in audio stream.", "WARN", "AudioDevice");
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
            if (buffer->Usage == BUFFER_USAGE_STATIC) {
                if (framesRead >= frameCount) break;
            } else if (isSubBufferProcessed[currentSubBufferIndex]) break;

            ma_uint32 totalFramesRemaining = frameCount - framesRead;
            if (totalFramesRemaining == 0) break;

            ma_uint32 framesRemainingInOutputBuffer;
            if (buffer->Usage == BUFFER_USAGE_STATIC) {
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
                    buffer->Stop();
                    break;
                }
            }
        }

        // Zero-fill excess
        ma_uint32 totalFramesRemaining = (frameCount - framesRead);
        if (totalFramesRemaining > 0) {
            memset((unsigned char *)pFramesOut + (framesRead*frameSizeInBytes), 0, totalFramesRemaining*frameSizeInBytes);

            // For static buffers, fill with silence for safety but dont report as read
            if (buffer->Usage != BUFFER_USAGE_STATIC) framesRead += totalFramesRemaining;
        }

        return framesRead;
    }

    void AudioDevice::__LogCallback(ma_context *pContext, ma_device *pDevice, ma_uint32 logLevel, const char *msg) {
        ConsoleMessage(std::string(msg), "ERR", "miniaudio");
    }

    void AudioDevice::__MixAudioFrames(float *framesOut_, const float *framesIn_, ma_uint32 frameCount_,
                                       float localVolume_) {
        for (ma_uint32 iFrame = 0; iFrame < frameCount_; ++iFrame) {
            for (ma_uint32 iChannel = 0; iChannel < _Device.playback.channels; ++iChannel) {
                float *frameOut = framesOut_ + (iFrame*_Device.playback.channels);
                const float *frameIn = framesIn_ + (iFrame*_Device.playback.channels);

                frameOut[iChannel] += (frameIn[iChannel]*_MasterVolume*localVolume_);
            }
        }
    }

    void AudioDevice::__SendAudioDataToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput,
                                              ma_uint32 frameCount) {
        if (!_Initialized) return;
        // Mixing is just an accumulation, init output buffer to 0
        memset(pFramesOut, 0, frameCount*pDevice->playback.channels*ma_get_bytes_per_sample(pDevice->playback.format));

        // Mutex for thread safety
        ma_mutex_lock(&_AudioLock);
        {
            for (auto buffer = _BufferFirst; buffer != nullptr; buffer = buffer->Next) {
                // Ignore stopped or paused sounds
                if (!buffer->Playing || buffer->Paused) continue;

                ma_uint32 framesRead = 0;

                while(true) {
                    if (framesRead > frameCount) {
                        ConsoleMessage("Mixed too many frames from audio buffer.", "WARN", "AudioDevice");
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
                            float *framesOut = (float *)pFramesOut + (framesRead*_Device.playback.channels);
                            float *framesIn = tmpBuffer;

                            __MixAudioFrames(framesOut, framesIn, framesJustRead, buffer->Volume);

                            framesToRead -= framesJustRead;
                            framesRead += framesJustRead;
                        }

                        if (framesJustRead < framesToReadRightNow) {
                            if (!buffer->Looping) {
                                buffer->Stop();
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
        ma_mutex_unlock(&_AudioLock);
    }

    void AudioDevice::__TrackAudioBuffer(TAudioBuffer *buffer_) {
        if (!_Initialized) return;
        ma_mutex_lock(&_AudioLock);
        {
            if (_BufferFirst == nullptr) _BufferFirst = buffer_;
            else {
                _BufferLast->Next = buffer_;
                buffer_->Prev = _BufferLast;
            }

            _BufferLast = buffer_;
        }
        ma_mutex_unlock(&_AudioLock);
    }

    void AudioDevice::__UntrackAudioBuffer(TAudioBuffer *buffer_) {
        if (!_Initialized) return;
        ma_mutex_lock(&_AudioLock);
        {
            if (buffer_->Prev == nullptr) _BufferFirst = buffer_->Next;
            else buffer_->Prev->Next = buffer_->Next;

            if (buffer_->Next == nullptr) _BufferLast = buffer_->Prev;
            else buffer_->Next->Prev = buffer_->Prev;

            buffer_->Prev = nullptr;
            buffer_->Next = nullptr;
        }
        ma_mutex_unlock(&_AudioLock);
    }

    // Public Methods

    void AudioDevice::CloseAudioBuffer(TAudioBuffer *buffer_) {
        if (!_Initialized) return;
        if (buffer_ != nullptr) {
            // Untrack buffer
            __UntrackAudioBuffer(buffer_);

            // Delete
            free(buffer_->Buffer);
            delete buffer_;
        }
    }

    void AudioDevice::Close() {
        if (_Initialized) {
            _Initialized = false;

            ma_mutex_uninit(&_AudioLock);
            ma_device_uninit(&_Device);
            ma_context_uninit(&_Context);

            ConsoleMessage("Audio device closed successfully.", "NOTICE", "AudioDevice");

        } else ConsoleMessage("Could not close audio device as it is not open.", "WARN", "AudioDevice");
    }

    TAudioBuffer *AudioDevice::InitAudioBuffer(ma_format format_, ma_uint32 channels_, ma_uint32 sampleRate_,
                                               ma_uint32 bufferSizeInFrames_, int usage_) {
        if (!_Initialized) return nullptr;
        auto *buffer = new TAudioBuffer();

        buffer->Buffer = calloc(bufferSizeInFrames_ * channels_ * ma_get_bytes_per_sample(format_), 1);

        if (buffer == nullptr || buffer->Buffer == nullptr) {
            ConsoleMessage("Failed to allocate memory for audio buffer.", "ERR", "AudioDevice");
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
        dspConfig.onRead = __AudioBufferDSPRead;
        dspConfig.pUserData = buffer;
        dspConfig.allowDynamicSampleRate = true;

        ma_result result = ma_pcm_converter_init(&dspConfig, &buffer->DSP);
        if (result != MA_SUCCESS) {
            ConsoleMessage("Failed to create data conversion pipeline.", "ERR", "AudioDevice");
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
        __TrackAudioBuffer(buffer);

        return buffer;
    }

    TAudioStream
    AudioDevice::InitAudioStream(unsigned int sampleRate_, unsigned int sampleSize_, unsigned int channels_) {
        TAudioStream stream;

        stream.SampleRate = sampleRate_;
        stream.SampleSize = sampleSize_;
        stream.Channels = channels_;

        ma_format formatIn = ((stream.SampleSize == 8) ? ma_format_u8 : ((stream.SampleSize == 16) ? ma_format_s16 : ma_format_f32));

        // The size of a streaming buffer must be at least double the size of a period
        unsigned int periodSize = _Device.playback.internalBufferSizeInFrames/_Device.playback.internalPeriods;
        unsigned int subBufferSize = AUDIO_BUFFER_SIZE;

        if (subBufferSize < periodSize) subBufferSize = periodSize;

        stream.Buffer = InitAudioBuffer(formatIn, stream.Channels, stream.SampleRate, subBufferSize*2, BUFFER_USAGE_STREAM);
        
        if (stream.Buffer != nullptr) {
            stream.Buffer->Looping = true; // Always loop streams
            ConsoleMessage("Audio stream loaded successfully.", "NOTICE", "AudioDevice");
        } else ConsoleMessage("Failed to create audio buffer.", "ERR", "AudioDevice");

        return stream;
    }

    void AudioDevice::Initialize() {
        // Cant run twice
        if (_Initialized) return;

        // Init context
        ma_context_config contextConfig = ma_context_config_init();
        contextConfig.logCallback = __LogCallback;

        ma_result result = ma_context_init(nullptr, 0, &contextConfig, &_Context);
        if (result != MA_SUCCESS) {
            ConsoleMessage("Failed to initialize audio context.", "ERR", "AudioDevice");
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
        config.dataCallback = __SendAudioDataToDevice;
        config.pUserData = nullptr;

        result = ma_device_init(&_Context, &config, &_Device);
        if (result != MA_SUCCESS) {
            ConsoleMessage("Failed to initialize audio playback device.", "ERR", "AudioDevice");
            ma_context_uninit(&_Context);
            return;
        }

        // Keep device running all the time.
        result = ma_device_start(&_Device);
        if (result != MA_SUCCESS) {
            ConsoleMessage("Failed to start audio playback device.", "ERR", "AudioDevice");
            ma_device_uninit(&_Device);
            ma_context_uninit(&_Context);
            return;
        }

        // Thread mixer
        result = ma_mutex_init(&_Context, &_AudioLock);
        if (result != MA_SUCCESS) {
            ConsoleMessage("Failed to create mutex for audio mixing.", "ERR", "AudioDevice");
            ma_device_uninit(&_Device);
            ma_context_uninit(&_Context);
            return;
        }

        ConsoleMessage("Audio device initialized successfully!", "NOTICE", "AudioDevice");
        ConsoleMessage("Audio backend: miniaudio/" + std::string(ma_get_backend_name(_Context.backend)), "NOTICE", "AudioDevice");
        ConsoleMessage("Audio format: " + std::string(ma_get_format_name(_Device.playback.format)) + " -> " + std::string(ma_get_format_name(_Device.playback.internalFormat)), "NOTICE", "AudioDevice");

        _Initialized = true;
    }

    bool AudioDevice::IsReady() {
        return _Initialized;
    }

    void AudioDevice::SetMasterVolume(float vol_) {
        _MasterVolume = vol_;
        if (_MasterVolume < 0.0f) _MasterVolume = 0.0f;
        if (_MasterVolume > 1.0f) _MasterVolume = 1.0f;
    }

    void AudioDevice::Update() {
        // We update here so we can add any future stuff
        TMusic::Update();
    }
}
