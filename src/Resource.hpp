/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
