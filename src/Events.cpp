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

#include "Events.hpp"

namespace ngine {
    namespace newEvents {
        EventMap EventDispatcher::m_events;

        void EventDispatcher::fire(const Event &e) {
            auto range = m_events.equal_range(&typeid(e));
            for (auto it = range.first; it != range.second; it++) {
                it->second(e);
            }
        }

        void EventDispatcher::_unbind(EventMap::iterator it) {
            m_events.erase(it);
        }

        void EventRef::remove() {
            if (!m_removed) {
                m_removeFunction();
                m_removed = true;
            }
        }

        EventRef::EventRef(std::function<void()> removeFunc) {
            m_removeFunction = std::move(removeFunc);
        }
    }
}