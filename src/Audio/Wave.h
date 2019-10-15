#ifndef WAVE_H
#define WAVE_H

#include "../Ngine.h"

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"

namespace NerdThings::Ngine::Audio {
    /*
     * Wave sound
     */
    struct NEAPI Wave : public IResource {
        // Public Fields

        /*
         * Number of audio channels
         */
        unsigned int Channels;

        /*
         * Audio data
         */
        void *Data;

        /*
         * Number of samples
         */
        unsigned int SampleCount;

        /*
         * Sample rate
         */
        unsigned int SampleRate;

        /*
         * Sample size
         */
        unsigned int SampleSize;

        // Destructor

        ~Wave();

        // Public Methods

        /*
         * Is wave sound valid?
         */
        bool IsValid() const override;

        /*
         * Load wave file
         */
        static Wave *LoadWave(const Filesystem::Path &path_);

        /*
         * Unload wave file
         */
        void Unload() override;

    private:
        // Private Methods

        void __LoadFLAC(const Filesystem::Path &path_);
        void __LoadMP3(const Filesystem::Path &path_);
        void __LoadOGG(const Filesystem::Path &path_);
        void __LoadWAV(const Filesystem::Path &path_);
    };
}

#endif //WAVE_H
