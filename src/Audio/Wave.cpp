/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "Wave.hpp"

#include <dr_flac.h>
#include <dr_mp3.h>
#include <dr_wav.h>
#include <stb_vorbis.h>

#include "../Logger.hpp"

namespace NerdThings::Ngine::Audio {
    Wave::~Wave() {
        Unload();
    }

    bool Wave::IsValid() const {
        return Channels > 0 && Data != nullptr;
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
        } else Logger::Error("Wave", "File format not supported.");

        return wav;
    }

    void Wave::Unload() {
        free(Data);
        Data = nullptr;
        Logger::Notice("Wave", "Unloaded wav data from RAM.");
    }

    void Wave::__LoadFLAC(const Filesystem::Path &path_) {
        // Decode entire FLAC file in one go
        uint64_t totalSampleCount;
        Data = drflac_open_file_and_read_pcm_frames_s16(path_.GetString().c_str(), &Channels, &SampleRate, &totalSampleCount);

        SampleCount = (unsigned int) totalSampleCount;
        SampleSize = 16;

        if (Channels > 2) Logger::Warn("Wave", "Too many FLAC channels!");

        if (Data == nullptr) Logger::Error("Wave", "Failed to load FLAC data.");
        else Logger::Notice("Wave", "Loaded FLAC file successfully!");
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
        if (Channels > 2) Logger::Error("Wave", "Too many MP3 channels!");

        if (Data == nullptr) Logger::Error("Wave", "Failed to load MP3 data.");
        else Logger::Notice("Wave", "Loaded MP3 file successfully!");
    }

    void Wave::__LoadOGG(const Filesystem::Path &path_) {
        // Load ogg file
        stb_vorbis *oggFile = stb_vorbis_open_filename(path_.GetString().c_str(), nullptr, nullptr);

        if (oggFile == nullptr) Logger::Warn("Wave", "OGG file could not be opened.");
        else {
            stb_vorbis_info info = stb_vorbis_get_info(oggFile);
            SampleRate = info.sample_rate;
            SampleSize = 16;
            Channels = info.channels;
            SampleCount = (unsigned int)stb_vorbis_stream_length_in_samples(oggFile)*info.channels;
            Data = (short *)malloc(SampleCount*Channels*sizeof(short));

            int numSamplesOgg = stb_vorbis_get_samples_short_interleaved(oggFile, info.channels, (short *)Data, SampleCount*Channels);
            stb_vorbis_close(oggFile);

            Logger::Notice("Wave", "OGG File loaded successfully!");
        }
    }

    void Wave::__LoadWAV(const Filesystem::Path &path_) {
        // Load wav file
        uint64_t totalFrameCount = 0;
        Data = drwav_open_file_and_read_pcm_frames_f32(path_.GetString().c_str(), &Channels, &SampleRate, &totalFrameCount);

        SampleCount = (int)totalFrameCount*Channels;
        SampleSize = 32;

        // Only support up to 2 channels
        if (Channels > 2) Logger::Error("Wave", "Too many WAV channels!");

        if (Data == nullptr) Logger::Error("Wave", "Failed to load WAV data.");
        else Logger::Notice("Wave", "Loaded WAV file successfully!");
    }
}