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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "../ngine.h"

#include <functional>

namespace NerdThings::Ngine::Core {
    // Forward declare for EventHandleRef
    template <typename ArgsType = EventArgs>
    class EventHandler;

    /*
     * A reference to an EventHandle attached to an EventHandler
     */
    template <typename ArgsType = EventArgs>
    struct EventHandleRef {
        // Public Fields

        /*
         * The handler that the handle is linked to
         */
        EventHandler<ArgsType>* AttachedHandler = nullptr;

        /*
         * The event handle ID
         */
        int ID = -1;

        // Public Methods

        /*
         * Unattach the handler
         */
        void UnBind() {
            AttachedHandler->UnBind(ID);
            ID = -1;
        }
    };

    /*
     * An event handler
     */
    template <typename ArgsType>
    class EventHandler {
        // Private Fields

        /*
         * All of the handles for the event handler
         */
        std::vector<std::function<void(ArgsType&)>> _Handles;

        /*
         * Unused indices in the Handles vector
         */
        std::vector<int> _UnusedIndices;

    public:
        // Public Methods

        /*
         * Bind a function
         */
        EventHandleRef<ArgsType> Bind(void (*func_)(ArgsType& e)) {
            auto f = std::bind(func_, std::placeholders::_1);
            if (_UnusedIndices.size() > 0) {
                auto id = _UnusedIndices.back();

                _Handles[id] = f;

                _UnusedIndices.erase(_UnusedIndices.end() - 1, _UnusedIndices.end());

                EventHandleRef<ArgsType> ref;
                ref.AttachedHandler = this;
                ref.ID = id;

                return ref;
            }

            _Handles.push_back(f);

            EventHandleRef<ArgsType> ref;
            ref.AttachedHandler = this;
            ref.ID = _Handles.size() - 1;

            return ref;
        }

        /*
         * Bind a class method
         */
        template <typename Class>
        EventHandleRef<ArgsType> Bind(Class* obj_, void (Class::*func_)(ArgsType& e)) {
            auto f = std::bind(func_, obj_, std::placeholders::_1);
            if (_UnusedIndices.size() > 0) {
                auto id = _UnusedIndices.back();

                _Handles[id] = f;

                _UnusedIndices.erase(_UnusedIndices.end() - 1, _UnusedIndices.end());

                EventHandleRef<ArgsType> ref;
                ref.AttachedHandler = this;
                ref.ID = id;

                return ref;
            }

            _Handles.push_back(f);

            EventHandleRef<ArgsType> ref;
            ref.AttachedHandler = this;
            ref.ID = _Handles.size() - 1;

            return ref;
        }

        /*
         * Clear all event handles
         */
        void Clear() {
            _Handles.clear();
        }

        /*
         * Invoke the handles attached to the event
         */
        void Invoke(ArgsType e) {
            for (auto i = 0; i < _Handles.size(); i++) {
                auto handle = _Handles[i];
                if (handle != nullptr) {
                    ArgsType sendE = e;
                    handle(sendE);

                    if (sendE.UnBind == true) {
                        UnBind(i);
                    }
                }
            }
        }

        void UnBind(int index) {
            if (index < 0)
                return;
            _Handles[index] = nullptr;
            _UnusedIndices.push_back(index);
        }
    };
}

#endif //EVENTHANDLER_H
