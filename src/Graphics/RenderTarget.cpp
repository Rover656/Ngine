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

#include "RenderTarget.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TRenderTarget::TRenderTarget(const int width_, const int height_) {
        const auto tmp = LoadRenderTexture(width_, height_);

        ID = tmp.id;
        Texture = TTexture2D::FromRaylibTex(tmp.texture);
        DepthBuffer = TTexture2D::FromRaylibTex(tmp.depth);
        DepthTexture = tmp.depthTexture;
    }

    TRenderTarget::TRenderTarget(TRenderTarget &&target_) {
        ID = target_.ID;
        Texture = std::move(target_.Texture);
        DepthBuffer = std::move(target_.DepthBuffer);
        DepthTexture = target_.DepthTexture;

        target_.ID = 0;
        DepthTexture = false;
    }

    // Destructor

    TRenderTarget::~TRenderTarget() {
        ConsoleMessage("Unloading and deleting render target.", "NOTICE", "RENDER TARGET");
        UnloadRenderTexture(ToRaylibTarget());
        ID = 0;
        Texture = std::shared_ptr<TTexture2D>(nullptr);
        DepthBuffer = std::shared_ptr<TTexture2D>(nullptr);
        DepthTexture = false;
    }

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    RenderTexture2D TRenderTarget::ToRaylibTarget() const {
        auto target = RenderTexture2D();

        target.id = ID;
        target.texture = Texture->ToRaylibTex();
        target.depth = DepthBuffer->ToRaylibTex();
        target.depthTexture = DepthTexture;

        return target;
    }

    TRenderTarget TRenderTarget::FromRaylibTarget(RenderTexture2D target_) {
        auto target = TRenderTarget();

        target.ID = target_.id;
        target.Texture = TTexture2D::FromRaylibTex(target_.texture);
        target.DepthBuffer = TTexture2D::FromRaylibTex(target_.depth);
        target.DepthTexture = target_.depthTexture;

        return target;
    }

    #endif

    // Operators

    TRenderTarget &TRenderTarget::operator=(TRenderTarget &&target_) {
        ID = target_.ID;
        Texture = std::move(target_.Texture);
        DepthBuffer = std::move(target_.DepthBuffer);
        DepthTexture = target_.DepthTexture;

        target_.ID = 0;
        DepthTexture = false;

        return *this;
    }

    bool TRenderTarget::operator==(const TRenderTarget &b) const {
        return ID == b.ID;
    }

    bool TRenderTarget::operator!=(const TRenderTarget &b) const {
        return ID != b.ID;
    }

}
