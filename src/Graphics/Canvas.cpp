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

#include "Canvas.h"

#include "Drawing.h"
#include "GraphicsManager.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructors

    Canvas::Canvas(float width_, float height_)
            : _Width(width_), _Height(height_) {
        _RenderTarget = new TRenderTarget(_Width, _Height);
    }

    // Destructor

    Canvas::~Canvas() {
        delete _RenderTarget;
    }

    // Public Methods

    void Canvas::Draw(Math::TVector2 pos_) {
        Graphics::Drawing::DrawTexture(&_RenderTarget->Texture,
                                       {
                                               pos_.X,
                                               pos_.Y,
                                               _Width,
                                               _Height
                                       },
                                       {
                                               0,
                                               0,
                                               static_cast<float>(_RenderTarget->Texture.Width),
                                               static_cast<float>(-_RenderTarget->Texture.Height)
                                       },
                                       Graphics::TColor::White);
    }

    float Canvas::GetHeight() {
        return _Height;
    }

    float Canvas::GetWidth() {
        return _Width;
    }

    void Canvas::ReDraw() {
        Graphics::GraphicsManager::PushTarget(_RenderTarget);
        RenderTargetRedraw();
        bool popped = false;
        Graphics::GraphicsManager::PopTarget(popped);
    }

    void Canvas::SetDimensions(float width_, float height_) {
        _Width = width_;
        _Height = height_;
        _RenderTarget = new TRenderTarget(_Width, _Height);
        ReDraw();
    }
}