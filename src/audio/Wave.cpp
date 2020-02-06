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

#include "audio/Wave.hpp"

#include <dr_flac.h>
#include <dr_mp3.h>
#include <dr_wav.h>
#include <stb_vorbis.h>

#include "Console.hpp"

namespace ngine::audio {
    Wave::~Wave() {
        free();
    }

    bool Wave::isValid() const {
        return Channels > 0 && Data != nullptr;
    }

    Wave *Wave::LoadWave(const filesystem::Path &path_) {
        auto wav = new Wave();

        if (path_.getFileExtension() == "mp3") {
            wav->_loadMP3(path_);
        } else if (path_.getFileExtension() == "wav") {
            wav->_loadWAV(path_);
        } else if (path_.getFileExtension() == "ogg") {
            wav->_loadOGG(path_);
        } else if (path_.getFileExtension() == "flac") {
            wav->_loadFLAC(path_);
        } else Console::Error("Wave", "File format not supported.");

        return wav;
    }

    void Wave::free() {
        ::free(Data);
        Data = nullptr;
        Console::Notice("Wave", "Unloaded wav data from RAM.");
    }

    void Wave::_loadFLAC(const filesystem::Path &path_) {
        // Decode entire FLAC file in one go
        uint64_t totalSampleCount;
        Data = drflac_open_file_and_read_pcm_frames_s16(path_.getString().c_str(), &Channels, &SampleRate, &totalSampleCount);

        SampleCount = (unsigned int) totalSampleCount;
        SampleSize = 16;

        if (Channels > 2) Console::Warn("Wave", "Too many FLAC channels!");

        if (Data == nullptr) Console::Error("Wave", "Failed to load FLAC data.");
        else Console::Notice("Wave", "Loaded FLAC file successfully!");
    }

    void Wave::_loadMP3(const filesystem::Path &path_) {
        // Decode whole MP3 file in one go
        uint64_t totalFrameCount = 0;
        drmp3_config config = {0};
        Data = drmp3_open_file_and_read_f32(path_.getString().c_str(), &config, &totalFrameCount);

        Channels = config.outputChannels;
        SampleRate = config.outputSampleRate;
        SampleCount = (int)totalFrameCount*Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2) Console::Error("Wave", "Too many MP3 channels!");

        if (Data == nullptr) Console::Error("Wave", "Failed to load MP3 data.");
        else Console::Notice("Wave", "Loaded MP3 file successfully!");
    }

    void Wave::_loadOGG(const filesystem::Path &path_) {
        // Load ogg file
        stb_vorbis *oggFile = stb_vorbis_open_filename(path_.getString().c_str(), nullptr, nullptr);

        if (oggFile == nullptr) Console::Warn("Wave", "OGG file could not be opened.");
        else {
            stb_vorbis_info info = stb_vorbis_get_info(oggFile);
            SampleRate = info.sample_rate;
            SampleSize = 16;
            Channels = info.channels;
            SampleCount = (unsigned int)stb_vorbis_stream_length_in_samples(oggFile)*info.channels;
            Data = (short *)malloc(SampleCount*Channels*sizeof(short));

            int numSamplesOgg = stb_vorbis_get_samples_short_interleaved(oggFile, info.channels, (short *)Data, SampleCount*Channels);
            stb_vorbis_close(oggFile);

            Console::Notice("Wave", "OGG File loaded successfully!");
        }
    }

    void Wave::_loadWAV(const filesystem::Path &path_) {
        // Load wav file
        uint64_t totalFrameCount = 0;
        Data = drwav_open_file_and_read_pcm_frames_f32(path_.getString().c_str(), &Channels, &SampleRate, &totalFrameCount);

        SampleCount = (int)totalFrameCount*Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2) Console::Error("Wave", "Too many WAV channels!");

        if (Data == nullptr) Console::Error("Wave", "Failed to load WAV data.");
        else Console::Notice("Wave", "Loaded WAV file successfully!");
    }
}