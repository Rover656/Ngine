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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "../ngine.h"

#include "RenderTarget.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Graphics management class
     */
    class NEAPI GraphicsManager {
        // Private fields

        /*
         * The render target stack
         */
        static std::vector<TRenderTarget*> _RenderTargetStack;
    public:
        // Public Methods

        /*
         * Pop a target off the stack (if present)
         * If not present, returns empty target
         */
        static TRenderTarget *PopTarget(bool &popped_);

        /*
         * Push a target onto the stack
         */
        static void PushTarget(TRenderTarget *target_);

        /*
         * Replace a target on the stack
         */
        static void ReplaceTarget(TRenderTarget *old_, TRenderTarget *new_);
    };
}

#endif //GRAPHICSMANAGER_H
