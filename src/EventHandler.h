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

#include "ngine.h"

#include <functional>

namespace NerdThings::Ngine {
    /*
     * Default event arguments
     */
    struct EventArgs {
        // Public Fields

        /*
         * Unbind event handle once run
         */
        bool UnBind = false;
    };

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
        EventHandler<ArgsType> *AttachedHandler = nullptr;

        /*
         * Attached function
         */
        std::function<void(ArgsType &)> Function;

        // Public Methods

        /*
         * Unattach the handler
         */
        void UnBind() {
            if (AttachedHandler != nullptr)
                AttachedHandler->UnBind(this);
            AttachedHandler = nullptr;
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
        std::vector<std::shared_ptr<EventHandleRef<ArgsType>>> _Handles;

        /*
         * Unused indices in the Handles vector
         */
        std::vector<int> _UnusedIndices;

    public:
        // Public Constructor(s)

        EventHandler<ArgsType>() = default;

        // Public Methods

        /*
         * Bind a function
         */
        EventHandleRef<ArgsType> *Bind(void (*func_)(ArgsType &e)) {
            auto f = std::bind(func_, std::placeholders::_1);
            if (_UnusedIndices.size() > 0) {
                auto id = _UnusedIndices.back();

                auto ref = std::make_shared<EventHandleRef<ArgsType>>();
                ref->AttachedHandler = this;
                ref->Function = f;

                _Handles[id] = ref;

                _UnusedIndices.erase(_UnusedIndices.end() - 1, _UnusedIndices.end());

                return ref.get();
            }

            auto ref = std::make_shared<EventHandleRef<ArgsType>>();
            ref->AttachedHandler = this;
            ref->Function = f;

            _Handles.push_back(ref);

            return ref.get();
        }

        /*
         * Bind a class method
         */
        template <typename Class>
        EventHandleRef<ArgsType> *Bind(Class *obj_, void (Class::*func_)(ArgsType &e)) {
            auto f = std::bind(func_, obj_, std::placeholders::_1);
            if (!_UnusedIndices.empty()) {
                auto id = _UnusedIndices.back();

                auto ref = std::make_shared<EventHandleRef<ArgsType>>();
                ref->AttachedHandler = this;
                ref->Function = f;

                _Handles[id] = ref;

                _UnusedIndices.erase(_UnusedIndices.end() - 1, _UnusedIndices.end());

                return ref.get();
            }

            auto ref = std::make_shared<EventHandleRef<ArgsType>>();
            ref->AttachedHandler = this;
            ref->Function = f;

            _Handles.push_back(ref);

            return ref.get();
        }

        /*
         * Clear all event handles
         */
        void Clear() {
            // Unbind all
            for (auto h : _Handles) h.UnBind();

            // Clear
            _Handles.clear();
        }

        /*
         * Invokes the handles attached to the event using a default constructor for the args
         */
        void Invoke() {
            Invoke({});
        }

        /*
         * Invoke the handles attached to the event
         */
        void Invoke(ArgsType e) {
            for (auto i = 0; i < _Handles.size(); i++) {
                auto handle = _Handles[i].get();
                ArgsType sendE = e;
                handle->Function(sendE);

                if (sendE.UnBind)
                    UnBind(handle);
            }
        }

        /*
         * Unbind an event handle by index
         */
        void UnBind(EventHandleRef<ArgsType> *handler_) {
            if (handler_ == nullptr)
                return;

            // Find and remove
            for (auto i = 0; i < _Handles.size(); i++) {
                if (_Handles[i].get() == handler_) {
                    _Handles[i] = nullptr;
                    _UnusedIndices.push_back(i);
                }
            }
        }

        // Operators

        void operator()() {
            Invoke();
        }

        void operator()(ArgsType e) {
            Invoke(e);
        }
    };
}

#endif //EVENTHANDLER_H
