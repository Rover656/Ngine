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

#ifdef _WIN32
#include "Windows.h"
#elif defined(__linux__)
#include <libgen.h>
#endif

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

    std::string WindowManager::GetExecutableDirectory() {
        // TODO: Finish adding from https://github.com/Quintus/pathie-cpp/blob/master/src/path.cpp#L927
#ifdef _WIN32
        char buf[MAX_PATH];
        if (GetModuleFileNameA(NULL, buf, MAX_PATH) == 0) {
            throw std::runtime_error("Failed to get path.");
        }
        return ::GetDirectoryPath(buf);
#elif defined(__linux__)
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        const char *path;
        if (count != -1) {
            path = dirname(result);
        }
        return std::string(path); // TODO: Test
#elif defined(__APPLE__)
        // TODO: Find some way of testing...
        char buf[PATH_MAX];
        uint32_t size = sizeof(buf);

        if (_NSGetExecutablePath(buf, &size) == 0)
            // Might contain symlinks or extra slashes. Shouldn't be an issue though
            // https://stackoverflow.com/questions/799679/programmatically-retrieving-the-absolute-path-of-an-os-x-command-line-app/1024933#1024933
            return ::GetDirectoryPath(buf);
        else
            throw std::runtime_error("An error occurred getting the executable location.");
#endif
        throw std::runtime_error("Need to add other platforms");
    }

    int WindowManager::GetWindowHeight() {
        return GetScreenHeight();
    }

    int WindowManager::GetWindowWidth() {
        return GetScreenWidth();
    }

    void WindowManager::HideCursor() {
        ::HideCursor();
    }

    void WindowManager::HideWindow() {
        ::HideWindow();
    }

    void WindowManager::Init(const int width_, const int height_, const std::string &title_) {
        InitWindow(width_, height_, title_.c_str());
    }

    bool WindowManager::IsCursorVisible() {
        return IsCursorHidden();
    }

    bool WindowManager::IsHidden() {
        return IsWindowHidden();
    }

    bool WindowManager::IsMinimized() {
        return IsWindowMinimized();
    }

    void WindowManager::SetMinSize(const int width_, const int height_) {
        ::SetWindowMinSize(width_, height_);
    }

    void WindowManager::SetMonitor(const int monitor_) {
        ::SetWindowMonitor(monitor_);
    }

    void WindowManager::SetPosition(const int x_, const int y_) {
        ::SetWindowPosition(x_, y_);
    }

    void WindowManager::SetSize(const int width_, const int height_) {
        ::SetWindowSize(width_, height_);
    }

    void WindowManager::SetTargetFPS(const int fps_) {
        ::SetTargetFPS(fps_);
    }

    void WindowManager::SetTitle(const std::string &title_) {
        ::SetWindowTitle(title_.c_str());
    }

    bool WindowManager::ShouldClose() {
        return WindowShouldClose();
    }

    void WindowManager::ShowCursor() {
        ::ShowCursor();
    }

    void WindowManager::ToggleFullscreen() {
        ::ToggleFullscreen();
    }

    void WindowManager::UnHideWindow() {
        ::UnhideWindow();
    }

}
