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

#include "GraphicsManager.h"

namespace NerdThings::Ngine::Graphics {
    // Private Fields

    std::vector<TRenderTarget*> GraphicsManager::_RenderTargetStack;

    // Public Methods

    TRenderTarget *GraphicsManager::PopTarget(bool &popped_) {
        if (!_RenderTargetStack.empty()) {
            // Get target
            auto pop = _RenderTargetStack.back();

            // Remove target
            _RenderTargetStack.pop_back();

            // Start using another if it is available
            if (!_RenderTargetStack.empty()) {
                BeginTextureMode(_RenderTargetStack.back()->ToRaylibTarget());
            }

            EndTextureMode();

            popped_ = true;
            return pop;
        }

        popped_ = false;
        return nullptr;
    }

    void GraphicsManager::PushTarget(TRenderTarget *target_) {
        // Add to target stack
        _RenderTargetStack.emplace_back(target_);

        // Use the target
        BeginTextureMode(target_->ToRaylibTarget());
    }

    void GraphicsManager::ReplaceTarget(TRenderTarget *old_, TRenderTarget *new_) {
        const auto oldPos = std::find(_RenderTargetStack.begin(), _RenderTargetStack.end(), old_) - _RenderTargetStack.
            begin();

        // If this is the currently active target, replace it too
        if (oldPos == _RenderTargetStack.size() - 1) {
            EndTextureMode();
            BeginTextureMode(new_->ToRaylibTarget());
        }

        // Send to stack
        _RenderTargetStack[oldPos] = new_;
    }
}
