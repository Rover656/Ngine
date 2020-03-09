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

#include "ngine/audio/Wave.hpp"

#include <dr_flac.h>
#include <dr_mp3.h>
#include <dr_wav.h>
#include <stb_vorbis.h>

#include "ngine/Console.hpp"

namespace ngine::audio {
    Wave::~Wave() { free(); }

    bool Wave::isValid() const { return Channels > 0 && Data != nullptr; }

    Wave *Wave::loadWave(const filesystem::Path &path) {
        auto wav = new Wave();

        if (path.getFileExtension() == "mp3") {
            wav->_loadMP3(path);
        } else if (path.getFileExtension() == "wav") {
            wav->_loadWAV(path);
        } else if (path.getFileExtension() == "ogg") {
            wav->_loadOGG(path);
        } else if (path.getFileExtension() == "flac") {
            wav->_loadFLAC(path);
        } else
            Console::error("Wave", "File format not supported.");

        return wav;
    }

    void Wave::free() {
        ::free(Data);
        Data = nullptr;
        Console::notice("Wave", "Unloaded wav data from RAM.");
    }

    void Wave::_loadFLAC(const filesystem::Path &path) {
        // Decode entire FLAC file in one go
        uint64_t totalSampleCount;
        Data = drflac_open_file_and_read_pcm_frames_s16(
            path.getString().c_str(), &Channels, &SampleRate,
            &totalSampleCount);

        SampleCount = (unsigned int)totalSampleCount;
        SampleSize = 16;

        if (Channels > 2)
            Console::warn("Wave", "Too many FLAC channels!");

        if (Data == nullptr)
            Console::error("Wave", "Failed to load FLAC data.");
        else
            Console::notice("Wave", "Loaded FLAC file successfully!");
    }

    void Wave::_loadMP3(const filesystem::Path &path) {
        // Decode whole MP3 file in one go
        uint64_t totalFrameCount = 0;
        drmp3_config config = {0};
        Data = drmp3_open_file_and_read_f32(path.getString().c_str(), &config,
                                            &totalFrameCount);

        Channels = config.outputChannels;
        SampleRate = config.outputSampleRate;
        SampleCount = (int)totalFrameCount * Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2)
            Console::error("Wave", "Too many MP3 channels!");

        if (Data == nullptr)
            Console::error("Wave", "Failed to load MP3 data.");
        else
            Console::notice("Wave", "Loaded MP3 file successfully!");
    }

    void Wave::_loadOGG(const filesystem::Path &path) {
        // Load ogg file
        stb_vorbis *oggFile = stb_vorbis_open_filename(path.getString().c_str(),
                                                       nullptr, nullptr);

        if (oggFile == nullptr)
            Console::warn("Wave", "OGG file could not be opened.");
        else {
            stb_vorbis_info info = stb_vorbis_get_info(oggFile);
            SampleRate = info.sample_rate;
            SampleSize = 16;
            Channels = info.channels;
            SampleCount =
                (unsigned int)stb_vorbis_stream_length_in_samples(oggFile) *
                info.channels;
            Data = (short *)malloc(SampleCount * Channels * sizeof(short));

            int numSamplesOgg = stb_vorbis_get_samples_short_interleaved(
                oggFile, info.channels, (short *)Data, SampleCount * Channels);
            stb_vorbis_close(oggFile);

            Console::notice("Wave", "OGG File loaded successfully!");
        }
    }

    void Wave::_loadWAV(const filesystem::Path &path) {
        // Load wav file
        uint64_t totalFrameCount = 0;
        Data = drwav_open_file_and_read_pcm_frames_f32(
            path.getString().c_str(), &Channels, &SampleRate, &totalFrameCount);

        SampleCount = (int)totalFrameCount * Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2)
            Console::error("Wave", "Too many WAV channels!");

        if (Data == nullptr)
            Console::error("Wave", "Failed to load WAV data.");
        else
            Console::notice("Wave", "Loaded WAV file successfully!");
    }
} // namespace ngine::audio