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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "../Ngine.h"

#include "RenderTarget.h"

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

#endif //GRAPHICSMANAGER_H
