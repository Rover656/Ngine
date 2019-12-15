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

#ifndef WAVE_HPP
#define WAVE_HPP

#include "../Config.hpp"

#include "../Filesystem/Filesystem.hpp"
#include "../Resource.hpp"

namespace NerdThings::Ngine::Audio {
    /**
     * Wave sound data.
     */
    struct NEAPI Wave : public IResource {
        /**
         * Number of audio channels.
         */
        unsigned int Channels;

        /**
         * Audio data.
         */
        void *Data;

        /**
         * Number of samples.
         */
        unsigned int SampleCount;

        /**
         * Sample rate.
         */
        unsigned int SampleRate;

        /**
         * Sample size.
         */
        unsigned int SampleSize;

        ~Wave();

        /**
         * Is wave sound valid?
         *
         * @return Whether or not the sound is valid.
         */
        bool IsValid() const override;

        /**
         * Load wave file.
         *
         * @param path_ The file to load wave data from.
         * @return The wave data.
         */
        static Wave *LoadWave(const Filesystem::Path &path_);

        /**
         * Unload wave file.
         */
        void Unload() override;

    private:
        void __LoadFLAC(const Filesystem::Path &path_);
        void __LoadMP3(const Filesystem::Path &path_);
        void __LoadOGG(const Filesystem::Path &path_);
        void __LoadWAV(const Filesystem::Path &path_);
    };
}

#endif //WAVE_HPP
