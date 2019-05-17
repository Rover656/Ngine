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

namespace Ngine::Core {
    /*
     * Default event arguments
     */
    struct EventArgs {
        // Contains nothing for the moment
    };

    /*
     * An event handler
     */
    template <typename ArgsType = EventArgs>
    class EventHandler {
        // Private Typedefs

        /*
         * An event handle function
         */
        typedef void (*EventHandle)(ArgsType e);

        // Private Fields

        /*
         * All of the handles for the event handler
         */
        std::vector<std::function<void(ArgsType)>> _Handles;

        /*
         * Unused indices in the Handles vector
         */
        std::vector<int> _UnusedIndices;

    public:
        // Public Methods

        /*
         * Bind a handler to the event
         */
        int Bind(std::function<void(ArgsType)> handle) {
            if (_UnusedIndices.size() > 0) {
                auto id = _UnusedIndices.back();

                _Handles[id] = handle;

                _UnusedIndices.erase(_UnusedIndices.end() - 1, _UnusedIndices.end());

                return id;
            }

            _Handles.push_back(handle);
            return _Handles.size() - 1;
        }

        template <typename Class>
        int Bind(Class* obj, void (Class::*func)(ArgsType e)) {
            auto f = std::bind(func, obj, std::placeholders::_1);
            if (_UnusedIndices.size() > 0) {
                auto id = _UnusedIndices.back();

                _Handles[id] = f;

                _UnusedIndices.erase(_UnusedIndices.end() - 1, _UnusedIndices.end());

                return id;
            }

            _Handles.push_back(f);
            return _Handles.size() - 1;
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
            for (const auto &handle : _Handles) {
                if (handle != nullptr)
                    handle(e);
            }
        }

        void UnBind(std::function<void(ArgsType)> handle) {
            _Handles.erase(std::remove(_Handles.begin(), _Handles.end(), handle), _Handles.end());
        }

        void UnBind(int index) {
            _Handles[index] = nullptr;
            _UnusedIndices.push_back(index);
        }
    };
}

#endif //EVENTHANDLER_H
