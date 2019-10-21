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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Ngine.h"

#include <functional>

namespace NerdThings::Ngine {
    /*
     * Base struct for event arguments (for events that need it)
     */
    struct EventArgs { };

    // Forward declare
    template<typename... ArgTypes>
    class Event;

    /*
     * Abstract event handler.
     */
    template<typename... ArgTypes>
    struct AbstractEventHandler {
        /*
         * Invoke the event handler.
         */
        virtual void Invoke(ArgTypes... args) = 0;

        void operator()(ArgTypes... args) {
            Invoke(args...);
        }
    };

    /*
     * An event handler that uses a static function.
     */
    template<typename... ArgTypes>
    struct FunctionEventHandler : public AbstractEventHandler<ArgTypes...> {
        /*
         * The function attached to to the handler.
         */
        void(*AttachedFunction)(ArgTypes...) = nullptr;

        /*
         * Create a null event handler.
         */
        FunctionEventHandler() {}

        /*
         * Create an event handler.
         */
        FunctionEventHandler(void(*func_)(ArgTypes...)) {
            AttachedFunction = func_;
        }

        void Invoke(ArgTypes... args) override {
            if (AttachedFunction != nullptr)
                AttachedFunction(args...);
        }
    };

    /*
     * An event handler that uses a class method.
     */
    template<typename Class, typename... ArgTypes>
    struct ClassMethodEventHandler : public AbstractEventHandler<ArgTypes...> {
        /*
         * The attached class.
         */
        Class *AttachedClass = nullptr;

        /*
         * The attached method
         */
        void (Class::*AttachedMethod)(ArgTypes...) = nullptr;

        /*
         * Initialize a null event handler.
         */
        ClassMethodEventHandler() {}

        /*
         * Create a class method event handler.
         */
        ClassMethodEventHandler(Class *obj_, void (Class::*func_)(ArgTypes...)) {
            AttachedClass = obj_;
            AttachedMethod = func_;
        }

        void Invoke(ArgTypes... args) override {
            if (AttachedClass != nullptr && AttachedMethod != nullptr)
                (AttachedClass->*AttachedMethod)(args...);
        }
    };

    /*
     * Contains persistent information about the attachment of an event handler.
     */
    template<typename... ArgTypes>
    struct EventAttachment {
    private:
        struct InternalAttachmentInformation {
            /*
             * The attached event.
             */
            Event<ArgTypes...> *AttachedEvent;

            /*
             * The handler this attachment is referring to.
             */
            AbstractEventHandler<ArgTypes...> *Handler;
        };
    public:
        /*
         * Information regarding the attachment.
         */
        std::shared_ptr<InternalAttachmentInformation> AttachmentInfo;

        /*
         * Create a null event handler attachment.
         */
        EventAttachment() {}

        /*
         * Create an event handler attachment.
         */
        EventAttachment(Event<ArgTypes...> * event_, AbstractEventHandler<ArgTypes...> *handler_) {
            // Create attachment info
            AttachmentInfo = std::make_shared<InternalAttachmentInformation>();
            AttachmentInfo->AttachedEvent = event_;
            AttachmentInfo->Handler = handler_;
        }

        /*
         * Detach the handler.
         */
        void Detach() {
            if (AttachmentInfo != nullptr) {
                if (AttachmentInfo->AttachedEvent != nullptr && AttachmentInfo->Handler != nullptr) {
                    // Detach from event
                    AttachmentInfo->AttachedEvent->Detach(AttachmentInfo->Handler);

                    // Delete internal attachment info
                    AttachmentInfo = nullptr;
                }
            }
        }

        /*
         * Determine if the event is attached.
         */
        bool IsAttached() {
            if (AttachmentInfo != nullptr) {
                if (AttachmentInfo->AttachedEvent != nullptr && AttachmentInfo->Handler != nullptr) {
                    auto attached = AttachmentInfo->AttachedEvent->IsAttached(AttachmentInfo->Handler);

                    // Delete internal attachment info if we aren't attached any more
                    if (!attached) AttachmentInfo = nullptr;

                    return attached;
                }
            }
            return false;
        }
    };

    /*
     * An event can be fired at any time. Handlers will be called with appropriate arguments when fired.
     */
    template<typename... ArgTypes>
    class Event {
        /*
         * All the attached handlers
         */
        std::vector<AbstractEventHandler<ArgTypes...> *> _Handlers;
    public:

        /*
         * Attach the event handler
         */
        EventAttachment<ArgTypes...> Attach(AbstractEventHandler<ArgTypes...> *handler_) {
            // Add
            _Handlers.push_back(handler_);

            // Return attachment information
            return {this, handler_};
        }

        /*
         * Clear the entire event.
         */
        void Clear() {
            for (auto handler : _Handlers) {
                Detach(handler);
            }
        }

        /*
         * Detach an event handler from the event.
         */
        void Detach(AbstractEventHandler<ArgTypes...> *handler_) {
            // Find in handler array
            auto iterator = std::find(_Handlers.begin(), _Handlers.end(), handler_);
            if (iterator == _Handlers.end())
                throw std::runtime_error("This event handler is not attached to this event.");

            // Remove from handlers vector
            _Handlers.erase(std::remove(_Handlers.begin(), _Handlers.end(), handler_), _Handlers.end());

            // Delete handler
            delete handler_;
        }

        /*
         * Invoke the handlers attached to the event
         */
        void Invoke(ArgTypes... args) {
            for (auto handler : _Handlers) {
                handler->Invoke(args...);
            }
        }

        bool IsAttached(AbstractEventHandler<ArgTypes...> *handler_) {
            auto iterator = std::find(_Handlers.begin(), _Handlers.end(), handler_);
            return iterator != _Handlers.end();
        }

        void operator()(ArgTypes... args) {
            Invoke(args...);
        }

        EventAttachment<ArgTypes...> operator+=(AbstractEventHandler<ArgTypes...> *handler_) {
            return Attach(handler_);
        }
    };
}

#endif //EVENTHANDLER_H
