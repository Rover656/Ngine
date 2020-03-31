#include "ngine/graphics/graphics_resource.hpp"

#include "ngine/graphics/graphics_device.hpp"

namespace ngine::graphics {
    void IGraphicsResource::free() {
        m_graphicsDevice->free(this);
    }

    ResourceType IGraphicsResource::getResourceType() const {
        return m_type;
    }

    IGraphicsResource::IGraphicsResource(IGraphicsDevice *graphicsDevice, ResourceType type)
            : m_graphicsDevice(graphicsDevice), m_type(type) {}

    IGraphicsResource::~IGraphicsResource() {
        // Free the resource.
        free();
    }
}