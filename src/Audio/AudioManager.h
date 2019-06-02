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

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "../ngine.h"

namespace NerdThings::Ngine::Audio {
    /*
     * Audio Manager
     */
    class NEAPI AudioManager {
        // Private Fields

        /*
         * Whether or not the device is initialized
         */
        static bool _Initialized;

    public:
        // Public Methods

        /*
         * Close audio device.
         * Usually called by the base game class, be careful with this
         */
        static void CloseDevice();

        /*
         * Init audio device.
         * Usually called by the game class, be careful with this
         */
        static void InitDevice();
    };
}

#endif //AUDIOMANAGER_H
