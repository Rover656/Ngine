#ifndef WAVE_H
#define WAVE_H

#include "../ngine.h"

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"

namespace NerdThings::Ngine::Audio {
    struct NEAPI TWave : public TResource {
        unsigned int Channels;
        void *Data;
        unsigned int SampleCount;
        unsigned int SampleRate;
        unsigned int SampleSize;

        // Destructor

        ~TWave();

        // Public Methods

        bool IsValid() const override;

        static TWave *LoadWave(const Filesystem::TPath &path_);

        void Unload() override;

    private:
        // Private Methods

        void __LoadFLAC(const Filesystem::TPath &path_);
        void __LoadMP3(const Filesystem::TPath &path_);
        void __LoadOGG(const Filesystem::TPath &path_);
        void __LoadWAV(const Filesystem::TPath &path_);
    };
}

#endif //WAVE_H
