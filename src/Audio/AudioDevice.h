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
    /*
     * Audio Manager
     */
    class NEAPI AudioDevice {
        // Private Fields

        /*
         * Active music streams
         */
        static std::vector<Music *> _ActiveMusic;

        /*
         * The audio mutex
         */
        static ma_mutex _AudioLock;

        /*
         * The first tracked buffer
         */
        static AudioBuffer *_BufferFirst;

        /*
         * The last tracked buffer
         */
        static AudioBuffer *_BufferLast;

        /*
         * Miniaudio context
         */
        static ma_context _Context;

        /*
         * Miniaudio device
         */
        static ma_device _Device;

        /*
         * Whether or not the device is initialized
         */
        static bool _Initialized;

        /*
         * Device master volume
         */
        static float _MasterVolume;

        // Private Methods

        /*
         * Read data from the converter
         */
        static ma_uint32 __AudioBufferDSPRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData);

        /*
         * Log miniaudio errors
         */
        static void __LogCallback(ma_context *pContext, ma_device *pDevice, ma_uint32 logLevel, const char* msg);

        /*
         * Audio mixer
         */
        static void __MixAudioFrames(float *framesOut_, const float *framesIn_, ma_uint32 frameCount_, float localVolume_);

        /*
         * Send audio data to the audio device
         */
        static void __SendAudioDataToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount);

        /*
         * Track an audio device
         */
        static void __TrackAudioBuffer(AudioBuffer *buffer_);

        /*
         * Untrack an audio device
         */
        static void __UntrackAudioBuffer(AudioBuffer *buffer_);
    public:
        // Public Methods

        /*
         * Close an audio buffer
         */
        static void CloseAudioBuffer(AudioBuffer *buffer_);

        /*
         * Close audio device.
         * Usually called by the base game class, be careful with this
         */
        static void Close();

        /*
         * Init audio buffer
         */
        static AudioBuffer *InitAudioBuffer(ma_format format_, ma_uint32 channels_, ma_uint32 sampleRate_, ma_uint32 bufferSizeInFrames_, int usage_);

        /*
         * Init audio stream
         */
        static AudioStream InitAudioStream(unsigned int sampleRate_, unsigned int sampleSize_, unsigned int channels_);

        /*
         * Init audio device.
         * Usually called by the game class, be careful with this
         */
        static void Initialize();

        /*
         * Is the device ready
         */
        static bool IsReady();

        /*
         * Set the master volume
         */
        static void SetMasterVolume(float vol_);

        /*
         * Update.
         * Handles music updating.
         */
        static void Update();
    };
}

#endif //AUDIODEVICE_H
