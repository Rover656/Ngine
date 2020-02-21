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

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "Config.hpp"

#include <functional>
#include <utility>

namespace ngine {
    class EventRef;

    /**
     * An event describes the content of a fired event.
     * Events do not support polymorphism, only hooking the exact type will work.
     */
    class Event {
    public:
        /**
         * The sender of the event.
         */
        void *Sender = nullptr;

        /**
         * Create a new base event.
         */
        Event(void *sender) : Sender(sender) {}
    };

    class BaseEventHook {
    public:
        void *Filter;

        BaseEventHook(void *senderFilter) : Filter(senderFilter) {}

        virtual ~BaseEventHook() = default;

        virtual void invoke(const Event &e) = 0;
    };

    template<typename TargetEvent>
    class EventHook : public BaseEventHook {
        std::function<void(const TargetEvent &)> m_func;
    public:
        EventHook(std::function<void(const TargetEvent &)> fn, void *senderFilter)
                : m_func(fn), BaseEventHook(senderFilter) {}

        void invoke(const Event &e) {
            m_func(static_cast<const TargetEvent &>(e));
        }
    };

    /**
     * Event reference map.
     */
    typedef std::multimap<const std::type_info *, std::shared_ptr<BaseEventHook>> EventRouteMap;

    class EventDispatcher {
    public:
        /**
         * Attach an event handler to an event.
         *
         * @tparam TargetEvent The event to listen for.
         * @param fn The function to fire.
         * @param senderFilter Filter for senders, this will allow you to only recieve events for a given sender.
         * @return A reference to the event binding.
         */
        template<typename TargetEvent>
        static EventRef bind(const std::function<void(const TargetEvent &e)> &fn, void *senderFilter = nullptr) {
            auto it = m_routes.emplace(&typeid(TargetEvent),
                                       std::static_pointer_cast<BaseEventHook>(
                                               std::make_shared<EventHook<TargetEvent>>(fn, senderFilter)));
            return EventRef(std::bind(_unbind, std::move(it)));
        }

        /**
         * Fire an event.
         *
         * @param e Event to fire.
         */
        template <typename EventType>
        static void fire(const EventType &e) {
            auto range = m_routes.equal_range(&typeid(EventType));
            for (auto it = range.first; it != range.second; it++) {
                if (it->second->Filter == nullptr || it->second->Filter == e.Sender)
                    it->second->invoke(static_cast<const Event &>(e));
            }
        }

    private:
        /**
         * The events map.
         */
        static EventRouteMap m_routes;

        /**
         * Unbind an event if you still have the function, otherwise use the `EventRef`.
         *
         * @param fn The function to remove
         */
        static void _unbind(EventRouteMap::iterator it);
    };

    class EventRef {
        friend class EventDispatcher;

    public:
        /**
         * Remove event from dispatcher.
         *
         * @warning If this is not called and the `EventRef` is destroyed, you cannot remove the function from the dispatcher.
         */
        void remove();

    private:
        /**
         * Function to call to remove the referenced handler.
         */
        std::function<void()> m_removeFunction;

        /**
         * Whether or not this has already been removed.
         */
        bool m_removed = false;

        /**
         * Create a new event reference.
         *
         * @param removeFunc Function to call on removal.
         */
        explicit EventRef(std::function<void()> removeFunc);
    };
}

#endif //EVENTS_HPP
