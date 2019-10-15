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

#include "Wave.h"

#include <dr_flac.h>
#include <dr_mp3.h>
#include <dr_wav.h>
#include <stb_vorbis.h>

namespace NerdThings::Ngine::Audio {
    Wave::~Wave() {
        Unload();
    }

    bool Wave::IsValid() const {
        return Data != nullptr; // TODO: Any more checks??
    }

    Wave *Wave::LoadWave(const Filesystem::Path &path_) {
        auto wav = new Wave();

        if (path_.GetFileExtension() == "mp3") {
            wav->__LoadMP3(path_);
        } else if (path_.GetFileExtension() == "wav") {
            wav->__LoadWAV(path_);
        } else if (path_.GetFileExtension() == "ogg") {
            wav->__LoadOGG(path_);
        } else if (path_.GetFileExtension() == "flac") {
            wav->__LoadFLAC(path_);
        } else ConsoleMessage("File format not supported.", "ERR", "Wave");

        return wav;
    }

    void Wave::Unload() {
        free(Data);
        Data = nullptr;
        ConsoleMessage("Unloaded wav data from RAM.", "NOTICE", "Wave");
    }

    void Wave::__LoadFLAC(const Filesystem::Path &path_) {
        // Decode entire FLAC file in one go
        uint64_t totalSampleCount;
        Data = drflac_open_file_and_read_pcm_frames_s16(path_.GetString().c_str(), &Channels, &SampleRate, &totalSampleCount);

        SampleCount = (unsigned int) totalSampleCount;
        SampleSize = 16;

        if (Channels > 2) ConsoleMessage("Too many FLAC channels!", "ERR", "Wave");

        if (Data == nullptr) ConsoleMessage("Failed to load FLAC data.", "ERR", "Wave");
        else ConsoleMessage("Loaded FLAC file successfully!", "NOTICE", "Wave");
    }

    void Wave::__LoadMP3(const Filesystem::Path &path_) {
        // Decode whole MP3 file in one go
        uint64_t totalFrameCount = 0;
        drmp3_config config = {0};
        Data = drmp3_open_file_and_read_f32(path_.GetString().c_str(), &config, &totalFrameCount);

        Channels = config.outputChannels;
        SampleRate = config.outputSampleRate;
        SampleCount = (int)totalFrameCount*Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2) ConsoleMessage("Too many MP3 channels!", "ERR", "Wave");

        if (Data == nullptr) ConsoleMessage("Failed to load MP3 data.", "ERR", "Wave");
        else ConsoleMessage("Loaded MP3 file successfully!", "NOTICE", "Wave");
    }

    void Wave::__LoadOGG(const Filesystem::Path &path_) {
        // Load ogg file
        stb_vorbis *oggFile = stb_vorbis_open_filename(path_.GetString().c_str(), nullptr, nullptr);

        if (oggFile == nullptr) ConsoleMessage("OGG file could not be opened.", "WARN", "Wave");
        else {
            stb_vorbis_info info = stb_vorbis_get_info(oggFile);

            SampleRate = info.sample_rate;
            SampleSize = 16;
            Channels = info.channels;
            SampleCount = (unsigned int)stb_vorbis_stream_length_in_samples(oggFile)*info.channels;

            Data = (short *)malloc(SampleCount*Channels*sizeof(short));

            int numSamplesOgg = stb_vorbis_get_samples_short_interleaved(oggFile, info.channels, (short *)Data, SampleCount*Channels);

            ConsoleMessage("OGG File loaded successfully!", "NOTICE", "Wave");

            stb_vorbis_close(oggFile);
        }
    }

    void Wave::__LoadWAV(const Filesystem::Path &path_) {
        // Load wav file
        uint64_t totalFrameCount = 0;
        Data = drwav_open_file_and_read_pcm_frames_f32(path_.GetString().c_str(), &Channels, &SampleRate, &totalFrameCount);

        SampleCount = (int)totalFrameCount*Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2) ConsoleMessage("Too many WAV channels!", "ERR", "Wave");

        if (Data == nullptr) ConsoleMessage("Failed to load WAV data.", "ERR", "Wave");
        else ConsoleMessage("Loaded WAV file successfully!", "NOTICE", "Wave");
    }
}