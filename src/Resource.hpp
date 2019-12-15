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

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "Config.hpp"

namespace NerdThings::Ngine {
    /**
     * Abstract class defining requirements for a resource.
     */
    class IResource {
    public:
        /**
         * Whether or not the resource is valid.
         *
         * @return Whether the resource is valid or not.
         */
        virtual bool IsValid() const = 0;

        /**
         * Unload this resource from memory.
         */
        virtual void Unload() = 0;
    };
}

#endif //RESOURCE_HPP
