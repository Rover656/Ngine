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

#include "Music.h"

#include <dr_mp3.h>
#include <dr_flac.h>
#include <stb_vorbis.h>

#include "AudioDevice.h"

namespace NerdThings::Ngine::Audio {
    // Destructor

    TMusic::~TMusic() {
        Unload();
    }

    // Public Methods

    float TMusic::GetLength() {
        return (float)SampleCount/(float)(Stream.SampleRate*Stream.Channels);
    }

    float TMusic::GetTimePlayed() {
        float secondsPlayed = 0.0f;

        unsigned int samplesPlayed = Stream.Buffer->TotalFramesProcessed*Stream.Channels;
        secondsPlayed = (float)samplesPlayed/(float)(Stream.SampleRate*Stream.Channels);

        return secondsPlayed;
    }

    bool TMusic::IsPlaying() const {
        return Stream.Buffer->IsPlaying();
    }

    bool TMusic::IsValid() const {
        return CTXData != nullptr;
    }

    TMusic *TMusic::LoadMusic(const Filesystem::TPath &path_) {
        auto music = new TMusic();
        bool loaded = false;

        if (path_.GetFileExtension() == "mp3") {
            auto ctxMp3 = (drmp3 *) malloc(sizeof(drmp3));
            music->CTXData = ctxMp3;

            int result = drmp3_init_file(ctxMp3, path_.GetString().c_str(), nullptr);

            if (result > 0) {
                music->CTXType = AUDIO_MP3;

                music->Stream = AudioDevice::InitAudioStream(ctxMp3->sampleRate, 32, ctxMp3->channels);
                music->SampleCount = drmp3_get_pcm_frame_count(ctxMp3) * ctxMp3->channels;
                music->LoopCount = 0;
                loaded = true;
            }
        } else if (path_.GetFileExtension() == "ogg") {
            // Open ogg audio stream
            music->CTXData = stb_vorbis_open_filename(path_.GetString().c_str(), nullptr, nullptr);

            if (music->CTXData != nullptr) {
                music->CTXType = AUDIO_OGG;
                stb_vorbis_info info = stb_vorbis_get_info((stb_vorbis *)music->CTXData);
                auto ctxOgg = (stb_vorbis *)music->CTXData;

                // OGG bit rate defaults to 16 bit, its enough for this compressed format
                music->Stream = AudioDevice::InitAudioStream(info.sample_rate, 16, info.channels);
                music->SampleCount = (unsigned int)stb_vorbis_stream_length_in_samples(ctxOgg)*info.channels;
                music->LoopCount = 0;
                loaded = true;
            }
        } else if (path_.GetFileExtension() == "flac") {
            music->CTXData = drflac_open_file(path_.GetString().c_str());

            if (music->CTXData != nullptr) {
                music->CTXType = AUDIO_FLAC;
                auto ctxFlac = (drflac *)music->CTXData;

                music->Stream = AudioDevice::InitAudioStream(ctxFlac->sampleRate, ctxFlac->bitsPerSample, ctxFlac->channels);
                music->SampleCount = (unsigned int)ctxFlac->totalSampleCount;
                music->LoopCount = 0;
                loaded = true;
            }
        }

        if (!loaded) {
            switch(music->CTXType) {
                case AUDIO_MP3: {
                    drmp3_uninit((drmp3 *) music->CTXData);
                    free(music->CTXData);
                } break;

                case AUDIO_OGG: {
                    stb_vorbis_close((stb_vorbis *)music->CTXData);
                } break;

                case AUDIO_FLAC: {
                    drflac_free((drflac *)music->CTXData);
                } break;
            }

            music->CTXData = nullptr;

            ConsoleMessage("Could not load music file.", "WARN", "Music");

            delete music;
            return nullptr;
        } else {
            ConsoleMessage("Successfully loaded music file.", "NOTICE", "Music");
            return music;
        }
    }

    void TMusic::Pause() {
        Stream.Buffer->Pause();
    }

    void TMusic::Play() {
        ma_uint32 curCursorPos = Stream.Buffer->FrameCursorPos;
        Stream.Buffer->Play();
        Stream.Buffer->FrameCursorPos = curCursorPos;

        // Add to active music
        auto it = std::find(_ActiveMusic.begin(), _ActiveMusic.end(), this);
        if (it == _ActiveMusic.end())
            _ActiveMusic.push_back(this);
    }

    void TMusic::Resume() {
        Stream.Buffer->Resume();
    }

    void TMusic::SetPitch(float pitch_) {
        Stream.Buffer->SetPitch(pitch_);
    }

    void TMusic::SetVolume(float vol_) {
        Stream.Buffer->SetVolume(vol_);
    }

    void TMusic::Stop() {
        Stream.Buffer->Stop();

        switch(CTXType) {
            case AUDIO_MP3: {
                drmp3_seek_to_pcm_frame((drmp3 *) CTXData, 0);
            } break;

            case AUDIO_OGG: {
                stb_vorbis_seek_start((stb_vorbis *)CTXData);
            } break;

            case AUDIO_FLAC: {
                drflac_seek_to_pcm_frame((drflac *)CTXData, 0);
            } break;
        }

        // Remove from active music
        _ActiveMusic.erase(std::remove(_ActiveMusic.begin(), _ActiveMusic.end(), this), _ActiveMusic.end());
    }

    void TMusic::Unload() {
        AudioDevice::CloseAudioBuffer(Stream.Buffer);

        switch(CTXType) {
            case AUDIO_MP3: {
                drmp3_uninit((drmp3 *) CTXData);
                free(CTXData);
            } break;

            case AUDIO_OGG: {
                stb_vorbis_close((stb_vorbis *) CTXData);
            } break;

            case AUDIO_FLAC: {
                drflac_free((drflac *) CTXData);
            } break;
        }

        CTXData = nullptr;
        ConsoleMessage("Unloaded music stream.", "NOTICE", "Music");
    }

    void TMusic::Update() {
        for (auto mus : _ActiveMusic) {
            mus->__Update();
        }
    }

    // Private Fields

    std::vector<TMusic *> TMusic::_ActiveMusic;

    // Private Methods

    void TMusic::__Update() {
        bool streamEnding = false;

        unsigned int subBufferSizeInFrames = Stream.Buffer->BufferSizeInFrames / 2;

        // Allocate pcm
        void *pcm = calloc(subBufferSizeInFrames * Stream.Channels * Stream.SampleSize / 8, 1);

        int samplesCount = 0;

        int sampleLeft = SampleCount - (Stream.Buffer->TotalFramesProcessed * Stream.Channels);

        while (Stream.Buffer != nullptr &&
               (Stream.Buffer->IsSubBufferProcessed[0] || Stream.Buffer->IsSubBufferProcessed[1])) {
            if ((sampleLeft / Stream.Channels) >= subBufferSizeInFrames) samplesCount = subBufferSizeInFrames *
                                                                                        Stream.Channels;
            else samplesCount = sampleLeft;

            switch (CTXType) {
                case AUDIO_MP3: {
                    drmp3_read_pcm_frames_f32((drmp3 *) CTXData, samplesCount / Stream.Channels, (float *) pcm);
                } break;

                case AUDIO_OGG: {
                    stb_vorbis_get_samples_short_interleaved((stb_vorbis *) CTXData, Stream.Channels, (short *)pcm, samplesCount);
                } break;

                case AUDIO_FLAC: {
                    drflac_read_pcm_frames_s16((drflac *) CTXData, samplesCount, (short *)pcm);
                } break;
            }

            Stream.UpdateStream(pcm, samplesCount);

            if ((CTXType == MODULE_XM) || (CTXType == MODULE_MOD)) {
                if (samplesCount > 1) sampleLeft -= samplesCount / 2;
                else sampleLeft -= samplesCount;
            } else sampleLeft -= samplesCount;

            if (sampleLeft <= 0) {
                streamEnding = true;
                break;
            }
        }

        // Free pcm data
        free(pcm);

        if (streamEnding) {
            // Stop music
            Stop();

            // Increase completed loops
            _LoopsCompleted++;

            // Decrease loop count to stop when required.
            if (_LoopsCompleted < LoopCount || LoopCount == 0) Play();

            // Reset if complete.
            if (_LoopsCompleted == LoopCount && LoopCount != 0) _LoopsCompleted = 0;
        } else {
            if (Stream.Buffer->IsPlaying()) Play();
        }
    }
}
