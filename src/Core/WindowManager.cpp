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

#include "WindowManager.h"

namespace NerdThings::Ngine::Core {
    // Public Methods

    void WindowManager::ApplyConfig(int config_) {
        // Build raylib flags
        unsigned char raylibFlags = 0;

        if (config_ & FULLSCREEN) raylibFlags |= FULLSCREEN;
        if (config_ & RESIZEABLE_WINDOW) raylibFlags |= RESIZEABLE_WINDOW;
        if (config_ & FRAMELESS_WINDOW) raylibFlags |= FRAMELESS_WINDOW;
        if (config_ & TRANSPARENT_WINDOW) raylibFlags |= TRANSPARENT_WINDOW;
        if (config_ & HIDDEN_WINDOW) raylibFlags |= HIDDEN_WINDOW;
        if (config_ & MSAA_4X) raylibFlags |= MSAA_4X;
        if (config_ & VSYNC) raylibFlags |= VSYNC;

        // Apply raylib flags
        SetConfigFlags(raylibFlags);
    }

    void WindowManager::Close() {
        CloseWindow();
    }

    int WindowManager::GetWindowHeight() {
        return GetScreenHeight();
    }

    int WindowManager::GetWindowWidth() {
        return GetScreenWidth();
    }

    void WindowManager::HideWindow() {
        ::HideWindow();
    }

    void WindowManager::Init(int width_, int height_, const std::string &title_) {
        InitWindow(width_, height_, title_.c_str());
    }

    bool WindowManager::IsHidden() {
        return IsWindowHidden();
    }

    bool WindowManager::IsMinimized() {
        return IsWindowMinimized();
    }

    void WindowManager::SetMinSize(int width_, int height_) {
        ::SetWindowMinSize(width_, height_);
    }

    void WindowManager::SetMonitor(int monitor_) {
        ::SetWindowMonitor(monitor_);
    }

    void WindowManager::SetPosition(int x_, int y_) {
        ::SetWindowPosition(x_, y_);
    }

    void WindowManager::SetSize(int width_, int height_) {
        ::SetWindowSize(width_, height_);
    }

    void WindowManager::SetTargetFPS(int fps_) {
        ::SetTargetFPS(fps_);
    }

    void WindowManager::SetTitle(const std::string &title_) {
        ::SetWindowTitle(title_.c_str());
    }

    void WindowManager::ToggleFullscreen() {
        ::ToggleFullscreen();
    }

    void WindowManager::UnhideWindow() {
        ::UnhideWindow();
    }

}
