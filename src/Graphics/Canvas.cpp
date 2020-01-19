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

#include "Graphics/Canvas.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Console.hpp"

namespace NerdThings::Ngine::Graphics {
    Canvas::Canvas(GraphicsDevice *graphicsDevice_, unsigned int width_, unsigned int height_)
            : m_width(width_), m_height(height_) {
        m_renderTarget = new RenderTarget(graphicsDevice_, m_width, m_height);
        m_graphicsDevice = graphicsDevice_;
    }

    Canvas::~Canvas() {
        Console::Notice("Canvas", "Deleting canvas.");
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
        m_graphicsDevice->PushTarget(m_renderTarget);
        renderer_->Clear(Color::Transparent);
        RenderTargetRedraw(renderer_);
        m_graphicsDevice->PopTarget();
#endif
    }

    unsigned int Canvas::GetWidth() {
        return m_width;
    }

    unsigned int Canvas::GetHeight() {
        return m_height;
    }

    void Canvas::SetDimensions(unsigned int width_, unsigned int height_) {
        Console::Notice("Canvas", "Resizing canvas. ReDraw is necessary.");
        delete m_renderTarget;
        m_width = width_;
        m_height = height_;
        m_renderTarget = new RenderTarget(m_graphicsDevice, m_width, m_height);
    }
}