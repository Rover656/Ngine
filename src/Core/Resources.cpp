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

#include "Resources.h"

// Platform specific includes
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <linux/limits.h>
#define _GNU_SOURCE
#define __USE_GNU
#include <dlfcn.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <pwd.h>
#endif

#include <filesystem>

namespace NerdThings::Ngine::Core {
    // Private Fields

    std::unordered_map<std::string, Graphics::TFont *> Resources::_Fonts;
    std::unordered_map<std::string, Audio::TMusic *> Resources::_Music;
    std::unordered_map<std::string, Audio::TSound *> Resources::_Sounds;
    std::unordered_map<std::string, Graphics::TTexture2D *> Resources::_Textures;

    // Public Methods

    void Resources::DeleteAll() {
        for (auto fnt : _Fonts) {
            delete fnt.second;
        }
        _Fonts.clear();

        for (auto mus : _Music) {
            delete mus.second;
        }
        _Music.clear();

        for (auto snd : _Sounds) {
            delete snd.second;
        }
        _Sounds.clear();

        for (auto tex : _Textures) {
            delete tex.second;
        }
        _Textures.clear();
    }

    void Resources::DeleteFont(const std::string &name_) {
        if (_Fonts.find(name_) != _Fonts.end()) {
            delete _Fonts[name_];
            _Fonts.erase(name_);
        }
    }

    void Resources::DeleteMusic(const std::string &name_) {
        if (_Music.find(name_) != _Music.end()) {
            delete _Music[name_];
            _Music.erase(name_);
        }
    }

    void Resources::DeleteSound(const std::string &name_) {
        if (_Sounds.find(name_) != _Sounds.end()) {
            delete _Sounds[name_];
            _Sounds.erase(name_);
        }
    }

    void Resources::DeleteTexture(const std::string &name_) {
        if (_Textures.find(name_) != _Textures.end()) {
            delete _Textures[name_];
            _Textures.erase(name_);
        }
    }

    std::string Resources::GetExecutableDirectory(bool &success_) {
        const auto exePath = GetExecutablePath(success_);

        if (success_) {
            return std::string(GetDirectoryPath(exePath.c_str()));
        }

        return "";
    }

    std::string Resources::GetExecutablePath(bool &success_) {
        // https://github.com/cginternals/cpplocate/blob/master/source/liblocate/source/liblocate.c#L39
        // I trust this works as there are no issues about it...
        success_ = false;
        #if defined(_WIN32) && defined(PLATFORM_DESKTOP)

        char exePath[MAX_PATH];
        unsigned int len = GetModuleFileNameA(GetModuleHandleA(0x0), exePath, MAX_PATH);
        if (len == 0)
            return "";

        success_ = true;
        return std::string(exePath);

        #elif defined(__linux__)

        char exePath[PATH_MAX];
        int len = readlink("/proc/self/exe", exePath, PATH_MAX);
        if (len <= 0 || len == PATH_MAX)
            return "";

        success_ = true;
        return std::string(exePath);

        #elif defined(__APPLE__)

        char exePath[PATH_MAX];

        unsigned int len = (unsigned int)PATH_MAX;

        if (_NSGetExecutablePath(exePath, &len) == 0)
        {
            char * realPath = realpath(exePath, 0x0);

            if (realPath == 0x0)
            {
                return "";
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            success_ = true;
            return pathStr;
        }
        else
        {
            char * intermediatePath = (char *)malloc(sizeof(char) * len);

            if (_NSGetExecutablePath(intermediatePath, &len) != 0)
            {
                free(intermediatePath);
                return "";
            }

            char * realPath = realpath(intermediatePath, 0x0);

            free(intermediatePath);

            if (realPath == 0x0)
            {
                return "";
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            success_ = true;
            return pathStr;
        }

        #else
        // Returns blank, this cannot be used
        return "";
        #endif
    }

    std::string Resources::GetFileExtension(const std::string &path_) {
        return std::string(::GetExtension(path_.c_str()));
    }

    Graphics::TFont *Resources::GetFont(const std::string &name_) {
        if (_Fonts.find(name_) != _Fonts.end())
            return _Fonts[name_];
        return nullptr;
    }

    Audio::TMusic *Resources::GetMusic(const std::string &name_) {
        if (_Music.find(name_) != _Music.end())
            return _Music[name_];
        return nullptr;
    }

    Audio::TSound *Resources::GetSound(const std::string &name_) {
        if (_Sounds.find(name_) != _Sounds.end())
            return _Sounds[name_];
        return nullptr;
    }

    Graphics::TTexture2D *Resources::GetTexture(const std::string &name_) {
        if (_Textures.find(name_) != _Textures.end())
            return _Textures[name_];
        return nullptr;
    }

    std::string Resources::GetWorkingDirectory() {
        return std::string(::GetWorkingDirectory());
    }

    void Resources::LoadDirectory(const std::string &directory_) {
        std::vector<std::string> files;

        // Get all files
        for (std::filesystem::recursive_directory_iterator i(directory_), end; i != end; ++i)
            if (!std::filesystem::is_directory(i->path())) {
                files.push_back(std::filesystem::relative(i->path(), directory_).string());
        }

        // File extension definitions
        std::vector<std::string> fntExts = {"ttf", "otf", "fnt"}; // TODO: Spritefont support
        std::vector<std::string> musExts = {"ogg", "flac", "mp3", "xm", "mod"};
        std::vector<std::string> sndExts = {"wav", "ogg", "flac", "mp3"};
        std::vector<std::string> texExts = {"png", "bmp", "tga", "gif", "pic", "psd", "hdr", "dds", "pkm", "ktx", "pvr", "astc"};


        // Load all files
        for (auto file : files) {
            auto name = file.substr(0, file.find_last_of("."));
            auto ext = GetFileExtension(file);

            auto actualPath = std::filesystem::path(directory_);
            actualPath /= file;

            if (std::find(fntExts.begin(), fntExts.end(), ext) != fntExts.end()) { // Font
                LoadFont(actualPath.string(), name);
            } else if (std::find(musExts.begin(), musExts.end(), ext) != musExts.end()) { // Music
                LoadMusic(actualPath.string(), name);
            } else if (std::find(sndExts.begin(), sndExts.end(), ext) != sndExts.end()) { // Sound
                LoadSound(actualPath.string(), name);
            } else if (std::find(texExts.begin(), texExts.end(), ext) != texExts.end()) { // Texture
                LoadTexture(actualPath.string(), name);
            }
        }
    }

    bool Resources::LoadFont(const std::string &inPath_, const std::string &name_) {
        auto fnt = Graphics::TFont::LoadFont(inPath_);
        if (fnt->Texture.ID > 0) {
            _Fonts.insert({ name_, fnt });
            return true;
        }
        return false;
    }

    bool Resources::LoadMusic(const std::string &inPath_, const std::string &name_) {
        auto mus = Audio::TMusic::LoadMusic(inPath_);
        if (mus->MusicData != nullptr) {
            _Music.insert({ name_, mus });
            return true;
        }
        return false;
    }

    bool Resources::LoadSound(const std::string &inPath_, const std::string &name_) {
        auto snd = Audio::TSound::LoadSound(inPath_);
        if (snd->AudioBuffer != nullptr) {
            _Sounds.insert({ name_, snd });
            return true;
        }
        return false;
    }

    bool Resources::LoadTexture(const std::string &inPath_, const std::string &name_) {
        auto tex = Graphics::TTexture2D::LoadTexture(inPath_);
        if (tex->ID > 0) {
            _Textures.insert({ name_, tex });
            return true;
        }
        return false;
    }
}
