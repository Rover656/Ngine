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
            : m_width(width_), m_height(height_) {
        m_renderTarget = new RenderTarget(m_width, m_height);
    }

    Canvas::~Canvas() {
        ConsoleMessage("Deleting canvas.", "NOTICE", "CANVAS");
        delete m_renderTarget;
    }

    void Canvas::Draw(Vector2 pos_) {
#ifndef USE_EXPERIMENTAL_RENDERER
        Graphics::Renderer::DrawTexture(m_renderTarget->GetTexture(),
                                        {
                                               pos_.X,
                                               pos_.Y,
                                               static_cast<float>(m_width),
                                               static_cast<float>(m_height)
                                       },
                                        {
                                               0,
                                               0,
                                               static_cast<float>(m_renderTarget->Width),
                                               static_cast<float>(m_renderTarget->Height) * -1
                                       },
                                        Graphics::Color::White);
#endif
    }

    void Canvas::ReDraw() {
#ifndef USE_EXPERIMENTAL_RENDERER
        Graphics::GraphicsManager::PushTarget(m_renderTarget);
        Graphics::Renderer::Clear(Color::Transparent);
        RenderTargetRedraw();
        Graphics::GraphicsManager::PopTarget();
#endif
    }

    unsigned int Canvas::GetWidth() {
        return m_width;
    }

    unsigned int Canvas::GetHeight() {
        return m_height;
    }

    void Canvas::SetDimensions(unsigned int width_, unsigned int height_) {
        ConsoleMessage("Resizing canvas.", "NOTICE", "CANVAS");
        delete m_renderTarget;
        m_width = width_;
        m_height = height_;
        m_renderTarget = new RenderTarget(m_width, m_height);
        ReDraw();
    }
}