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

#ifndef BASECAMERA_H
#define BASECAMERA_H

namespace NerdThings::Ngine {
    /*
     * Camera interface
     */
    struct IBaseCamera {
        // Public Methods

        /*
         * Begin using camera
         */
        virtual void BeginCamera() const = 0;

        /*
         * End using camera
         */
        virtual void EndCamera() const = 0;
    };
}

#endif //BASECAMERA_H
