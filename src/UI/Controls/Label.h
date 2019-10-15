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

#ifndef LABEL_H
#define LABEL_H

#include "../../Ngine.h"

#include "../../Graphics/Renderer.h"
#include "../UIControlSized.h"
#include "../UIPanel.h"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A text label
     */
    class Label : public UIControlSized {
    private:
        // Private Fields

        /*
         * The font to be used
         */
        Graphics::Font *_Font = nullptr;

        /*
         * The font size
         */
        float _FontSize = 12;

        /*
         * Font spacing
         */
        float _FontSpacing = 2;

        /*
         * Whether or not the label is resized based on content or not
         */
        bool _IsFixedSize = false;

        /*
         * The text in the label
         */
        std::string _Text;

        // Private Methods

        void SetDynamicSize() {
            auto style = GetStyle();

            auto size = _Font->MeasureString(_Text, _FontSize, _FontSpacing);
            SetHeight(size.Y + _FontSpacing + style.Padding[0] + style.Padding[2]);
            SetWidth(size.X + _FontSpacing + style.Padding[1] + style.Padding[3]);
        }

    public:
        // Public Constructor(s)

        /*
         * Create a label
         */
        Label(std::string text_, Graphics::Font *font_)
                : UIControlSized(), _Text(std::move(text_)), _Font(font_), _IsFixedSize(false) {
            SetDynamicSize();
        }

        Label(std::string text_, Graphics::Font *font_, float width_, float height_)
                : UIControlSized(), _Text(std::move(text_)), _Font(font_), _IsFixedSize(true) {
            SetWidth(width_);
            SetHeight(height_);
        }

        // Public Methods

        void Draw() override {
            auto style = GetStyle();

            UIControl::Draw();

            auto controlContentRect = style.GetContentRect(GetRenderRectangle());

            if (_Font != nullptr && style.DrawDefaults)
                Graphics::Renderer::DrawTextRect(_Font, _Text, controlContentRect, _FontSize, _FontSpacing,
                                                 style.ForeColor); // TODO: Wordwrap option
        }

        Graphics::Font *GetFont() {
            return _Font;
        }

        float GetFontSize() {
            return _FontSize;
        }

        float GetFontSpacing() {
            return _FontSpacing;
        }

        bool GetIsFixedSize() {
            return _IsFixedSize;
        }

        std::string GetText() {
            return _Text;
        }

        void SetFont(Graphics::Font *font_) {
            _Font = font_;
            if (!_IsFixedSize)
                SetDynamicSize();
        }

        void SetFontSize(float size_) {
            _FontSize = size_;
            if (!_IsFixedSize)
                SetDynamicSize();
        }

        void SetFontSpacing(float spacing_) {
            _FontSpacing = spacing_;
            if (!_IsFixedSize)
                SetDynamicSize();
        }

        void SetHeight(float height_) override {
            UIControlSized::SetHeight(height_);
        }

        void SetStyle(TUIStyle style_) override {
            UIControl::SetStyle(style_);
            if (!_IsFixedSize)
                SetDynamicSize();
        }

        void SetIsFixedSize(bool fixedSize_) {
            _IsFixedSize = fixedSize_;
            if (!_IsFixedSize)
                SetDynamicSize();
        }

        void SetText(std::string text_) {
            _Text = std::move(text_);
            if (!_IsFixedSize)
                SetDynamicSize();
        }

        void SetWidth(float width_) override {
            UIControlSized::SetWidth(width_);
        }
    };
}

#endif // LABEL_H
