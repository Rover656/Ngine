#ifndef RESOURCE_H
#define RESOURCE_H

#include "Ngine.h"

namespace NerdThings::Ngine {
    /*
     * Abstract class defining requirements for a resource.
     */
    class IResource {
    public:
        // Public Methods

        /*
         * Whether or not the resource is valid.
         */
        virtual bool IsValid() const = 0;

        /*
         * Unload this resource from memory.
         */
        virtual void Unload() = 0;
    };
}

#endif //RESOURCE_H
