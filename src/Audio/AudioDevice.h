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

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include "../ngine.h"

#include "../../third-party/miniaudio/miniaudio.h"
#include "../EventHandler.h"
#include "AudioBuffer.h"
#include "Music.h"
#include "Sound.h"

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 44100

#define MAX_AUDIO_BUFFER_POOL_CHANNELS 16

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
        static std::vector<TMusic *> _ActiveMusic;

        static TAudioBuffer *_AudioBufferPool[];

        static unsigned int _AudioBufferPoolCounter;

        static unsigned int _AudioBufferPoolChannels[];

        static ma_mutex _AudioLock;

        static TAudioBuffer *_BufferFirst;

        static TAudioBuffer *_BufferLast;

        static ma_context _Context;

        static ma_device _Device;

        /*
         * Whether or not the device is initialized
         */
        static bool _Initialized;

        static float _MasterVolume;

        // Private Methods

        static ma_uint32 __AudioBufferDSPRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData);

        static void __CloseBufferPool();

        static void __InitBufferPool();

        static void __LogCallback(ma_context *pContext, ma_device *pDevice, ma_uint32 logLevel, const char* msg);

        static void __MixAudioFrames(float *framesOut_, const float *framesIn_, ma_uint32 frameCount_, float localVolume_);

        static void __SendAudioDataToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount);

        static void __TrackAudioBuffer(TAudioBuffer *buffer_);

        static void __UntrackAudioBuffer(TAudioBuffer *buffer_);
    public:
        // Public Methods

        /*
         * Close an audio buffer
         */
        static void CloseAudioBuffer(TAudioBuffer *buffer_);

        /*
         * Close audio device.
         * Usually called by the base game class, be careful with this
         */
        static void Close();

        /*
         * Get the length of a music stream
         */
        static float GetMusicLength(TMusic *mus_);

        /*
         * Get the amount of time of a music stream played
         */
        static float GetMusicTimePlayed(TMusic *mus_);

        /*
         * Init audio buffer
         */
        static TAudioBuffer *InitAudioBuffer(ma_format format_, ma_uint32 channels_, ma_uint32 sampleRate_, ma_uint32 bufferSizeInFrames_, int usage_);

        /*
         * Init audio stream
         */
        static TAudioStream InitAudioStream(unsigned int sampleRate_, unsigned int sampleSize_, unsigned int channels_);

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
