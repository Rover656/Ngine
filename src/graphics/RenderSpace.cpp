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

#include "graphics/RenderSpace.hpp"

#include "graphics/Renderer.hpp"
#include "Console.hpp"

namespace ngine::graphics {
    float RenderableNode::GetDepth() {
        return m_depth;
    }

    Texture2D *RenderableNode::GetTexture() {
        return m_texture;
    }

    ShaderProgram *RenderableNode::GetShader() {
        return m_shader;
    }

    void RenderSpace::_sortNodes() {
        // Define our sorting predicates
        auto sortFunc = [this](RenderableNode *a, RenderableNode *b) {
            return (a->GetDepth() < b->GetDepth()) // Sort by depth
                    || (a->GetDepth() < b->GetDepth() && a->GetShader() < b->GetShader())  // If same depth, sort by shader
                    || (a->GetDepth() < b->GetDepth() && a->GetShader() == b->GetShader() && a->GetTexture() < b->GetTexture()); // If same depth and shader, sort by texture
        };

        // Sort
        std::sort(m_nodes.begin(), m_nodes.end(), sortFunc);

        // No longer "dirty"
        m_dirty = false;
    }

    void RenderSpace::addNode(RenderableNode *node) {
        // Add to the vector then asks for re-sort
        m_nodes.push_back(node);
        m_dirty = true;
    }

    void RenderSpace::removeNode(RenderableNode *node) {
        // Remove from the vector (does not dirty as it doesnt break order)
        m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
    }

    Camera *RenderSpace::createCamera(const std::string &name) {
        if (getCamera(name) == nullptr) {
            m_cameras[name] = Camera();
            return &m_cameras[name];
        }

        // We already have a camera by this name TODO: Throw?
        return nullptr;
    }

    Camera *RenderSpace::getCamera(const std::string &name) {
        auto it = m_cameras.find(name);
        if (it != m_cameras.end()) {
            return &m_cameras[name];
        }
        return nullptr;
    }

    void RenderSpace::render(Renderer *renderer, const std::string &cameraName, RenderTarget *target) {
        // Check if we need to re-sort
        if (m_dirty) _sortNodes();

        // Get camera
        auto camera = getCamera(cameraName);
        if (camera == nullptr)
            Console::Fail("RenderSpace", "Camera does not exist.");

        // TODO: Camera based culling system.

        // Get graphics device and enable camera and target
        auto graphicsDevice = renderer->getGraphicsDevice();
        camera->beginCamera(graphicsDevice);
        if (target != nullptr)
            graphicsDevice->pushTarget(target);

        // Render each node
        for (auto node : m_nodes) {
            node->Render(renderer);
        }

        // Clean
        renderer->renderBatch();
        camera->endCamera(graphicsDevice);
        if (target != nullptr)
            graphicsDevice->popTarget();
    }
}