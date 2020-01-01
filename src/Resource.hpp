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
