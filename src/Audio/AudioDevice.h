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

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include "../Ngine.h"

#include "../../third-party/miniaudio/miniaudio.h"
#include "../EventHandler.h"
#include "AudioBuffer.h"
#include "Music.h"
#include "Sound.h"

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 44100

#define AUDIO_BUFFER_SIZE 4096

namespace NerdThings::Ngine::Audio {
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
        static ma_uint32 _audioBufferDSPRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData);

        /**
         * Log miniaudio errors.
         *
         * @param pContext Miniaudio context.
         * @param pDevice Miniaudio device.
         * @param logLevel Log/Error level.
         * @param msg Log message.
         */
        static void _logCallback(ma_context *pContext, ma_device *pDevice, ma_uint32 logLevel, const char* msg);

        /**
         * Audio mixer.
         *
         * @param framesOut_ Frame output.
         * @param framesIn_ Frame input.
         * @param frameCount_ Frame count.
         * @param localVolume_ Local/Master volume.
         */
        static void _mixAudioFrames(float *framesOut_, const float *framesIn_, ma_uint32 frameCount_, float localVolume_);

        /**
         * Send audio data to the audio device.
         *
         * @param pDevice Device to send to.
         * @param pFramesOut Frames out.
         * @param pFramesInput Frames in.
         * @param frameCount Frame count.
         */
        static void _sendAudioDataToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount);

        /**
         * Track an audio buffer.
         *
         * @param buffer_ Buffer to track
         */
        static void _trackAudioBuffer(AudioBuffer *buffer_);

        /**
         * Untrack an audio buffer.
         *
         * @param buffer_ Buffer to stop tracking.
         */
        static void _untrackAudioBuffer(AudioBuffer *buffer_);
    public:
        /**
         * Close an audio buffer
         *
         * @param buffer_ Buffer to close.
         */
        static void CloseAudioBuffer(AudioBuffer *buffer_);

        /**
         * Close the audio device.
         *
         * @warning Normally called by Game, don't use unless reimplementing Game or reinitialising audio.
         */
        static void Close();

        /**
         * Init audio buffer.
         *
         * @param format_ Audio format
         * @param channels_ Channel number.
         * @param sampleRate_ Sample rate.
         * @param bufferSizeInFrames_ Buffer size (frames).
         * @param usage_ Buffer usage.
         * @return A new audio buffer.
         */
        static AudioBuffer *InitAudioBuffer(ma_format format_, ma_uint32 channels_, ma_uint32 sampleRate_, ma_uint32 bufferSizeInFrames_, int usage_);

        /**
         * Init audio stream.
         *
         * @param sampleRate_ Sample rate.
         * @param sampleSize_ Sample size.
         * @param channels_ Channel count.
         * @return A new audio stream.
         */
        static AudioStream InitAudioStream(unsigned int sampleRate_, unsigned int sampleSize_, unsigned int channels_);

        /**
         * Init audio device.
         *
         * @note This is normally called by the Game. Should use with care.
         */
        static void Initialize();

        /**
         * Determine if the device is ready.
         *
         * @return Whether the device is ready or not.
         */
        static bool IsReady();

        /**
         * Set the master volume.
         *
         * @param vol_ The new master volume.
         */
        static void SetMasterVolume(float vol_);

        /**
         * Update.
         *
         * @note This is just for updating audio streams and buffers.
         */
        static void Update();
    };
}

#endif //AUDIODEVICE_H
