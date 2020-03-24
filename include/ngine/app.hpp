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

#ifndef NGINE_APP_HPP
#define NGINE_APP_HPP

#include "ngine/config.hpp"

namespace ngine {
    /**
     * The App is the entrypoint to any program made with Ngine.
     * Normally, game developers will use `Game` as it implements Game behaviour already.
     * This is here to allow Ngine's use within programs (such as editors etc.)
     */
    class NEAPI App {
    public:
        virtual void run() = 0;
        virtual void init() = 0;
        virtual void cleanup() = 0;
        virtual void draw() = 0;
        virtual void update() = 0;
    };
}

#endif //NGINE_APP_HPP
