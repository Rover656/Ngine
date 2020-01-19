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

#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include "../Config.hpp"

#include "RenderTarget.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * Graphics management class.
     */
    class NEAPI GraphicsManager {
        /**
         * The render target stack.
         */
        static std::vector<RenderTarget *> m_renderTargetStack;

        /**
         * The current viewport width.
         */
        static unsigned int m_currentWidth;

        /**
         * The current viewport height.
         */
        static unsigned int m_currentHeight;

        /**
         * Configure the framebuffer for use with the render target.
         *
         * @param target_ Target to configure for.
         */
        static void _useRenderTarget(RenderTarget *target_);

        /**
         * Reset the framebuffer to defaults.
         */
        static void _endRenderTarget();
    public:
        /**
         * Gets the current framebuffer width.
         *
         * @return The current framebuffer width.
         */
        static unsigned int GetCurrentWidth();

        /**
         * Gets the current render height.
         *
         * @return The current framebuffer height.
         */
        static unsigned int GetCurrentHeight();

        /**
         * Loads the default orthographic matrix and viewport.
         */
        static void SetupFramebuffer();

        /**
         * Push a target onto the stack.
         * This target will begin to be used immediately.
         */
        static void PushTarget(RenderTarget *target_);

        /**
         * Pop a target off the stack (if present).
         * If there is no render target, we will now be rendering directly to the framebuffer.
         */
        static void PopTarget();

        /**
         * Replace a target on the stack with another.
         * Will immediately start using this target if it is currently active.
         *
         * @param old_ The target to be replaced.
         * @param new_ The target to replace the old.
         */
        static void ReplaceTarget(RenderTarget *old_, RenderTarget *new_);
    };
}

#endif //GRAPHICSMANAGER_HPP
