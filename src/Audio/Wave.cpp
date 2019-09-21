#include "Wave.h"

#include <dr_mp3.h>
#include <dr_wav.h>

namespace NerdThings::Ngine::Audio {
    TWave::~TWave() {
        Unload();
    }

    bool TWave::IsValid() const {
        return Data != nullptr; // TODO: Any more checks??
    }

    TWave *TWave::LoadWave(const Filesystem::TPath &path_) {
        auto wav = new TWave();

        if (path_.GetFileExtension() == "mp3") {
            wav->__LoadMP3(path_);
        } else if (path_.GetFileExtension() == "wav") {
            wav->__LoadWAV(path_);
        } else ConsoleMessage("File format not supported.", "ERR", "Wave");

        return wav;
    }

    void TWave::Unload() {
        free(Data);
        Data = nullptr;
        ConsoleMessage("Unloaded wav data from RAM.", "NOTICE", "Wave");
    }

    void TWave::__LoadMP3(const Filesystem::TPath &path_) {
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

    void TWave::__LoadWAV(const Filesystem::TPath &path_) {
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