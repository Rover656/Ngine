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

#ifndef RENDERSPACE_HPP
#define RENDERSPACE_HPP

#include "../Config.hpp"

#include "Camera.hpp"
#include "Renderer.hpp"
#include "RenderTarget.hpp"
#include "ShaderProgram.hpp"

namespace ngine::graphics {
    /**
     * An object that can be rendered to the screen.
     *
     * @todo Add the ability for raw vertices and indices of any primitive to be controlled by this. Will be keyed by Z coordinate.
     */
    class NEAPI RenderableNode {
        /**
         * The depth is the 2D z-index.
         */
        float m_depth;

        /**
         * The texture used for rendering this node.
         */
        Texture2D *m_texture;

        /**
         * The shader used for rendering this node.
         */
        ShaderProgram *m_shader;
    public:
        float GetDepth();
        Texture2D *GetTexture();
        ShaderProgram *GetShader();

        /**
         * Draw the renderable to the current `RenderSpace`.
         */
        virtual void Render(Renderer *renderer_) = 0;
    };

    /**
     * A render space manages a wide range of nodes which render using the immediate mode renderer, but order them so that they will appear correctly on the screen.
     * A render space can handle 2D and 3D nodes in one single render call, ordering every node properly, ready to be sent to the renderer.
     */
    class NEAPI RenderSpace {
        /**
         * All renderable objects that we need to process.
         */
        std::vector<RenderableNode *> m_nodes;

        /**
         * Whether or not the nodes need resorting
         */
        bool m_dirty = false;

        /**
         * List of all cameras
         */
        std::map<std::string, Camera> m_cameras;

        /**
         * Sort all of the nodes.
         */
        void _sortNodes();
    public:
        /**
         * Add a renderable node to the space.
         *
         * @param node_ The node to add.
         */
        void addNode(RenderableNode *node_);

        /**
         * Remove a renderable node from the space.
         *
         * @param node_ The node to remove.
         */
        void removeNode(RenderableNode *node_);

        /**
         * Create a new named camera.
         *
         * @param name_ The name of the camera.
         * @return The new camera.
         */
        Camera *createCamera(const std::string &name_);

        /**
         * Get a named camera.
         *
         * @param name_ The camera to get.
         * @return The camera or null if not found.
         */
        Camera *getCamera(const std::string &name_);

        /**
         * Render everything in this space with the given camera optionally to a render target.
         *
         * @param renderer_ The renderer to use to put this into the target.
         * @param camera_ The camera to use.
         * @param target_ Texture to render to or null.
         */
        void render(Renderer *renderer_, const std::string &camera_, RenderTarget *target_ = nullptr);
    };
}

#endif // RENDERSPACE_HPP
