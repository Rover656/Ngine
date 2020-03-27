#include "ngine/graphics/graphics_resource.hpp"

#include "ngine/graphics/graphics_device.hpp"

namespace ngine::graphics {
    void GraphicsResource::free() {
        m_graphicsDevice->free(this);
    }

    ResourceType GraphicsResource::getResourceType() const {
        return m_type;
    }

    GraphicsResource::GraphicsResource(GraphicsDevice *graphicsDevice, ResourceType type)
            : m_graphicsDevice(graphicsDevice), m_type(type) {}

    GraphicsResource::~GraphicsResource() {
        // Free the resource.
        free();
    }
}