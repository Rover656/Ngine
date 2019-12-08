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

#ifndef EVENTS_H
#define EVENTS_H

#include "Ngine.h"

#include <functional>

namespace NerdThings::Ngine {
    /**
     * Base structure for event arguments.
     */
    struct EventArgs { };

    // Forward declare
    template<typename... ArgTypes>
    class Event;

    /**
     * Abstract event handler.
     *
     * @tparam ArgTypes The argument types the event will pass to the handler.
     */
    template<typename... ArgTypes>
    struct AbstractEventHandler {
        /**
         * Invoke the event handler.
         *
         * @param args Arguments to be passed to the callback
         */
        virtual void Invoke(ArgTypes... args) = 0;

        void operator()(ArgTypes... args) {
            Invoke(args...);
        }
    };

    /**
     * An event handler that uses a static function as a callback.
     *
     * @tparam ArgTypes The argument types the event will pass to the handler.
     */
    template<typename... ArgTypes>
    struct FunctionEventHandler : public AbstractEventHandler<ArgTypes...> {
        /**
         * The function attached to to the handler.
         */
        void(*AttachedFunction)(ArgTypes...) = nullptr;

        /**
         * Create an empty event handler.
         */
        FunctionEventHandler() {}

        /**
         * Create an event handler.
         *
         * @param func_ The function to be used as the callback
         */
        FunctionEventHandler(void(*func_)(ArgTypes...)) {
            AttachedFunction = func_;
        }

        void Invoke(ArgTypes... args) override {
            if (AttachedFunction != nullptr)
                AttachedFunction(args...);
        }
    };

    /**
     * An event handler that uses a class method as a callback.
     *
     * @tparam Class The class the callback method is a member of.
     * @tparam ArgTypes The argument types the event will pass to the handler.
     */
    template<typename Class, typename... ArgTypes>
    struct ClassMethodEventHandler : public AbstractEventHandler<ArgTypes...> {
        /**
         * The attached class.
         */
        Class *AttachedClass = nullptr;

        /**
         * The attached method
         */
        void (Class::*AttachedMethod)(ArgTypes...) = nullptr;

        /**
         * Initialize an empty event handler.
         */
        ClassMethodEventHandler() {}

        /**
         * Create a class method event handler.
         *
         * @param obj_ The class whose member method we will use.
         * @param func_ The class method we will use.
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

    /**
     * Contains persistent information about the attachment of an event handler.
     *
     * @tparam ArgTypes The argument types the event will pass to the handler.
     */
    template<typename... ArgTypes>
    struct EventAttachment {
    private:
        /**
         * The structure that contains information about the attachment.
         */
        struct InternalAttachmentInformation {
            /**
             * The attached event.
             */
            Event<ArgTypes...> *AttachedEvent;

            /**
             * The handler this attachment is referring to.
             */
            AbstractEventHandler<ArgTypes...> *Handler;
        };
    public:
        /**
         * Information regarding the attachment.
         */
        std::shared_ptr<InternalAttachmentInformation> AttachmentInfo;

        /**
         * Create an empty event handler attachment.
         */
        EventAttachment() {}

        /**
         * Create an event handler attachment.
         *
         * @param event_ The event we are following attachment to.
         * @param handler_ The handlerer we are following.
         */
        EventAttachment(Event<ArgTypes...> * event_, AbstractEventHandler<ArgTypes...> *handler_) {
            // Create attachment info
            AttachmentInfo = std::make_shared<InternalAttachmentInformation>();
            AttachmentInfo->AttachedEvent = event_;
            AttachmentInfo->Handler = handler_;
        }

        /**
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

        /**
         * Determine if the event is attached.
         *
         * @return Whether or not the event handler is still attached to the event.
         */
        bool IsAttached() const {
            if (AttachmentInfo != nullptr) {
                if (AttachmentInfo->AttachedEvent != nullptr && AttachmentInfo->Handler != nullptr) {
                    return AttachmentInfo->AttachedEvent->IsAttached(AttachmentInfo->Handler);
                }
            }
            return false;
        }
    };

    /**
     * An event can be fired at any time. Handlers will be called with appropriate arguments when fired.
     *
     * @tparam ArgTypes The argument types the event will pass to the handler.
     */
    template<typename... ArgTypes>
    class Event {
        /**
         * All the attached handlers
         */
        std::vector<AbstractEventHandler<ArgTypes...> *> _Handlers;
    public:
        /**
         * Attach the event handler
         *
         * @param handler_ The event handler to be attached.
         * @return The event attachment information.
         */
        EventAttachment<ArgTypes...> Attach(AbstractEventHandler<ArgTypes...> *handler_) {
            // Add
            _Handlers.push_back(handler_);

            // Return attachment information
            return {this, handler_};
        }

        /**
         * Clear the entire event.
         */
        void Clear() {
            for (auto handler : _Handlers) {
                Detach(handler);
            }
        }

        /**
         * Detach an event handler from the event.
         *
         * @param handler_ The handler to be detached.
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

        /**
         * Invoke the handlers attached to the event
         *
         * @param args The arguments to pass to the handlers.
         */
        void Invoke(ArgTypes... args) {
            for (auto handler : _Handlers) {
                handler->Invoke(args...);
            }
        }

        /**
         * Whether or not the handler is attached to us.
         *
         * @param handler_ The handler to check.
         * @return If the handler is attached.
         */
        bool IsAttached(AbstractEventHandler<ArgTypes...> *handler_) const {
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

#endif //EVENTS_H
