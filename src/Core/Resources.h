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

#ifndef RESOURCES_H
#define RESOURCES_H

#include "../ngine.h"

namespace NerdThings::Ngine::Core {
    /*
     * Resource management class
     * WIP. Currently only has path functions
     */
    class NEAPI Resources {
    public:

        // Public Methods

        static std::string GetExecutableDirectory(bool &success_);

        static std::string GetExecutablePath(bool &success_);
    };
}

#endif //RESOURCES_H
