/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "Canvas.hpp"

#include "../Logger.hpp"
#include "Renderer.hpp"
#include "GraphicsManager.hpp"

namespace NerdThings::Ngine::Graphics {
    Canvas::Canvas(unsigned int width_, unsigned int height_)
            : m_width(width_), m_height(height_) {
        m_renderTarget = new RenderTarget(m_width, m_height);
    }

    Canvas::~Canvas() {
        Logger::Notice("Canvas", "Deleting canvas.");
        delete m_renderTarget;
    }

    void Canvas::Draw(Graphics::Renderer *renderer_, Vector2 pos_) {
        m_renderTarget->GetTexture()->Draw(
                                        renderer_,
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
    }

    void Canvas::ReDraw(Graphics::Renderer *renderer_) {
#ifndef USE_EXPERIMENTAL_RENDERER
        Graphics::GraphicsManager::PushTarget(m_renderTarget);
        renderer_->Clear(Color::Transparent);
        RenderTargetRedraw(renderer_);
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
        Logger::Notice("Canvas", "Resizing canvas. ReDraw is necessary.");
        delete m_renderTarget;
        m_width = width_;
        m_height = height_;
        m_renderTarget = new RenderTarget(m_width, m_height);
    }
}