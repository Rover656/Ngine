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

#ifndef AUDIODEVICE_HPP
#define AUDIODEVICE_HPP

#include "../Config.hpp"

#include "../../third-party/miniaudio/miniaudio.h"
#include "../Events.hpp"
#include "AudioBuffer.hpp"
#include "Music.hpp"
#include "Sound.hpp"

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 44100

#define AUDIO_BUFFER_SIZE 4096

namespace ngine::audio {
    /**
     * Audio device control
     */
    class NEAPI AudioDevice {
        /**
         * Active music streams
         */
        static std::vector<Music *> m_activeMusic;

        /**
         * The audio mutex
         */
        static ma_mutex m_audioLock;

        /**
         * The first tracked buffer
         */
        static AudioBuffer *m_bufferFirst;

        /**
         * The last tracked buffer
         */
        static AudioBuffer *m_bufferLast;

        /**
         * Miniaudio context
         */
        static ma_context m_context;

        /**
         * Miniaudio device
         */
        static ma_device m_device;

        /**
         * Whether or not the device is initialized
         */
        static bool m_initialized;

        /**
         * Device master volume
         */
        static float m_masterVolume;

        /**
         * Read data from the converter.
         *
         * @param pDSP PCM converter
         * @param pFramesOut Frame output array
         * @param frameCount Frame count.
         * @param pUserData Userdata.
         * @return The number of frames read.
         */
        static ma_uint32
        _audioBufferDSPRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData);

        /**
         * Log miniaudio errors.
         *
         * @param pContext Miniaudio context.
         * @param pDevice Miniaudio device.
         * @param logLevel Log/Error level.
         * @param msg Log message.
         */
        static void _logCallback(ma_context *pContext, ma_device *pDevice, ma_uint32 logLevel, const char *msg);

        /**
         * Audio mixer.
         *
         * @param framesOut Frame output.
         * @param framesIn Frame input.
         * @param frameCount Frame count.
         * @param localVolume Local/Master volume.
         */
        static void
        _mixAudioFrames(float *framesOut, const float *framesIn, ma_uint32 frameCount, float localVolume);

        /**
         * Send audio data to the audio device.
         *
         * @param pDevice Device to send to.
         * @param pFramesOut Frames out.
         * @param pFramesInput Frames in.
         * @param frameCount Frame count.
         */
        static void
        _sendAudioDataToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount);

        /**
         * Track an audio buffer.
         *
         * @param buffer Buffer to track
         */
        static void _trackAudioBuffer(AudioBuffer *buffer);

        /**
         * Untrack an audio buffer.
         *
         * @param buffer Buffer to stop tracking.
         */
        static void _untrackAudioBuffer(AudioBuffer *buffer);

    public:
        /**
         * Close an audio buffer
         *
         * @param buffer Buffer to close.
         */
        static void closeAudioBuffer(AudioBuffer *buffer);

        /**
         * Close the audio device.
         *
         * @warning Normally called by Game, don't use unless reimplementing Game or reinitialising audio.
         */
        static void close();

        /**
         * Init audio buffer.
         *
         * @param format Audio format
         * @param channels Channel number.
         * @param sampleRate Sample rate.
         * @param bufferSizeInFrames Buffer size (frames).
         * @param usage Buffer usage.
         * @return A new audio buffer.
         */
        static AudioBuffer *
        initAudioBuffer(ma_format format, ma_uint32 channels, ma_uint32 sampleRate, ma_uint32 bufferSizeInFrames,
                        AudioBufferUsage usage);

        /**
         * Init audio stream.
         *
         * @param sampleRate Sample rate.
         * @param sampleSize Sample size.
         * @param channels Channel count.
         * @return A new audio stream.
         */
        static AudioStream initAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels);

        /**
         * Init audio device.
         *
         * @note This is normally called by the Game. Should use with care.
         */
        static void initialize();

        /**
         * Determine if the device is ready.
         *
         * @return Whether the device is ready or not.
         */
        static bool isReady();

        /**
         * Set the master volume.
         *
         * @param vol The new master volume.
         */
        static void setMasterVolume(float vol);

        /**
         * Update.
         *
         * @note This is just for updating audio streams and buffers.
         */
        static void update();
    };
}

#endif //AUDIODEVICE_HPP
