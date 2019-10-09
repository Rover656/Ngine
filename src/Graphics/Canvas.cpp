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

#include "Renderer.h"
#include "GraphicsManager.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructors

    Canvas::Canvas(unsigned int width_, unsigned int height_)
            : _Width(width_), _Height(height_) {
        _RenderTarget = new RenderTarget(_Width, _Height);
    }

    // Destructor

    Canvas::~Canvas() {
        ConsoleMessage("Deleting canvas.", "NOTICE", "CANVAS");
        delete _RenderTarget;
    }

    // Public Methods

    void Canvas::Draw(Vector2 pos_) {
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
    }

    unsigned int Canvas::GetHeight() {
        return _Height;
    }

    unsigned int Canvas::GetWidth() {
        return _Width;
    }

    void Canvas::ReDraw() {
        Graphics::GraphicsManager::PushTarget(_RenderTarget);
        Graphics::Renderer::Clear(Color::Transparent);
        RenderTargetRedraw();
        bool popped = false;
        Graphics::GraphicsManager::PopTarget(popped);
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