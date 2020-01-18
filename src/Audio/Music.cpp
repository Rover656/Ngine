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

#include "Music.hpp"

#include <dr_mp3.h>
#include <dr_flac.h>
#include <stb_vorbis.h>

#include "../Console.hpp"
#include "AudioDevice.hpp"

namespace NerdThings::Ngine::Audio {
    // Destructor

    Music::~Music() {
        Unload();
    }

    // Public Methods

    float Music::GetLength() {
        return (float)SampleCount/(float)(Stream.SampleRate*Stream.Channels);
    }

    float Music::GetTimePlayed() {
        float secondsPlayed = 0.0f;

        unsigned int samplesPlayed = Stream.Buffer->TotalFramesProcessed*Stream.Channels;
        secondsPlayed = (float)samplesPlayed/(float)(Stream.SampleRate*Stream.Channels);

        return secondsPlayed;
    }

    bool Music::IsPlaying() const {
        return Stream.Buffer->IsPlaying();
    }

    bool Music::IsValid() const {
        return CTXData != nullptr;
    }

    Music *Music::LoadMusic(const Filesystem::Path &path_) {
        auto music = new Music();
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
            // Free music
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
            delete music;

            Console::Warn("Music", "Could not load music file.");
            return nullptr;
        } else {
            Console::Notice("Music", "Successfully loaded music file.");
            return music;
        }
    }

    void Music::Pause() {
        Stream.Buffer->Pause();
    }

    void Music::Play() {
        ma_uint32 curCursorPos = Stream.Buffer->FrameCursorPos;
        Stream.Buffer->Play();
        Stream.Buffer->FrameCursorPos = curCursorPos;

        // Add to active music
        auto it = std::find(_ActiveMusic.begin(), _ActiveMusic.end(), this);
        if (it == _ActiveMusic.end())
            _ActiveMusic.push_back(this);
    }

    void Music::Resume() {
        Stream.Buffer->Resume();
    }

    void Music::SetPitch(float pitch_) {
        Stream.Buffer->SetPitch(pitch_);
    }

    void Music::SetVolume(float vol_) {
        Stream.Buffer->SetVolume(vol_);
    }

    void Music::Stop() {
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

    void Music::Unload() {
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
        Console::Notice("Music", "Unloaded music stream.");
    }

    void Music::Update() {
        for (auto mus : _ActiveMusic) {
            mus->__Update();
        }
    }

    // Private Fields

    std::vector<Music *> Music::_ActiveMusic;

    // Private Methods

    void Music::__Update() {
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
