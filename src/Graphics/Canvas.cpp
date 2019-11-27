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

#include "Canvas.h"

#include "Renderer.h"
#include "GraphicsManager.h"

namespace NerdThings::Ngine::Graphics {
    Canvas::Canvas(unsigned int width_, unsigned int height_)
            : _Width(width_), _Height(height_) {
        _RenderTarget = new RenderTarget(_Width, _Height);
    }

    Canvas::~Canvas() {
        ConsoleMessage("Deleting canvas.", "NOTICE", "CANVAS");
        delete _RenderTarget;
    }

    void Canvas::Draw(Vector2 pos_) {
#ifndef USE_EXPERIMENTAL_RENDERER
        Graphics::Renderer::DrawTexture(_RenderTarget->GetTexture(),
                                       {
                                               pos_.X,
                                               pos_.Y,
                                               static_cast<float>(_Width),
                                               static_cast<float>(_Height)
                                       },
                                       {
                                               0,
                                               0,
                                               static_cast<float>(_RenderTarget->Width),
                                               static_cast<float>(_RenderTarget->Height) * -1
                                       },
                                       Graphics::Color::White);
#endif
    }

    void Canvas::ReDraw() {
#ifndef USE_EXPERIMENTAL_RENDERER
        Graphics::GraphicsManager::PushTarget(_RenderTarget);
        Graphics::Renderer::Clear(Color::Transparent);
        RenderTargetRedraw();
        Graphics::GraphicsManager::PopTarget();
#endif
    }

    unsigned int Canvas::GetWidth() {
        return _Width;
    }

    unsigned int Canvas::GetHeight() {
        return _Height;
    }

    void Canvas::SetDimensions(unsigned int width_, unsigned int height_) {
        ConsoleMessage("Resizing canvas.", "NOTICE", "CANVAS");
        delete _RenderTarget;
        _Width = width_;
        _Height = height_;
        _RenderTarget = new RenderTarget(_Width, _Height);
        ReDraw();
    }
}