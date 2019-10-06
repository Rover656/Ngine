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

#include <regex>
#include <sstream>

namespace NerdThings::Ngine::Filesystem {
    // Private Fields

    std::unordered_map<std::string, std::unique_ptr<Graphics::Font>> Resources::_Fonts;
    std::unordered_map<std::string, std::unique_ptr<Audio::Music>> Resources::_Music;
    std::unordered_map<std::string, std::unique_ptr<Audio::Sound>> Resources::_Sounds;
    std::unordered_map<std::string, std::unique_ptr<Graphics::Texture2D>> Resources::_Textures;

    // Public Fields

    Path Resources::ResourcesDirectory = Path("content");

    // Public Methods

    void Resources::DeleteAll() {
        _Fonts.clear();
        _Music.clear();
        _Sounds.clear();
        _Textures.clear();
    }

    void Resources::DeleteFont(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Fonts.find(name_) != _Fonts.end()) {
            _Fonts.erase(name_);
        }
    }

    void Resources::DeleteMusic(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Music.find(name_) != _Music.end()) {
            _Music.erase(name_);
        }
    }

    void Resources::DeleteSound(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Sounds.find(name_) != _Sounds.end()) {
            _Sounds.erase(name_);
        }
    }

    void Resources::DeleteTexture(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Textures.find(name_) != _Textures.end()) {
            _Textures.erase(name_);
        }
    }

    Graphics::Font *Resources::GetFont(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Fonts.find(name_) != _Fonts.end())
            return _Fonts[name_].get();
        return nullptr;
    }

    Audio::Music *Resources::GetMusic(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Music.find(name_) != _Music.end())
            return _Music[name_].get();
        return nullptr;
    }

    Audio::Sound *Resources::GetSound(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Sounds.find(name_) != _Sounds.end())
            return _Sounds[name_].get();
        return nullptr;
    }

    Graphics::Texture2D *Resources::GetTexture(const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (_Textures.find(name_) != _Textures.end())
            return _Textures[name_].get();
        return nullptr;
    }

    void Resources::LoadResources() {
        // Get content dir
        auto contentDir = Directory(Path::GetExecutableDirectory() / ResourcesDirectory);

        // Get all files
        auto files = contentDir.GetFilesRecursive();

        // File extension definitions
        std::vector<std::string> fntExts = {"ttf", "otf"};
        std::vector<std::string> musExts = {"ogg", "flac", "mp3"};//, "xm", "mod"};
        std::vector<std::string> sndExts = {"wav", "ogg", "flac", "mp3"};
        std::vector<std::string> texExts = {"png", "bmp", "tga", "gif", "pic", "psd"};

        // Load all files
        for (auto file : files) {
            // Get name (relative path to the content folder)
            auto name = file.GetObjectPath().GetRelativeTo(contentDir.GetObjectPath()).GetStringNoExtension();

            // Replace windows slashes with forward slashes
            name = std::regex_replace(name, std::regex("\\\\"), "/");

            // Get extension
            auto ext = file.GetFileExtension();

            // Get actual path
            auto path = file.GetObjectPath();

            // Load resources
            if (std::find(fntExts.begin(), fntExts.end(), ext) != fntExts.end()) { // Font
                LoadFont(path, name);
            }

            if (std::find(musExts.begin(), musExts.end(), ext) != musExts.end()) { // Music
                LoadMusic(path, name);
            }

            if (std::find(sndExts.begin(), sndExts.end(), ext) != sndExts.end()) { // Sound
                LoadSound(path, name);
            }

            if (std::find(texExts.begin(), texExts.end(), ext) != texExts.end()) { // Texture
                LoadTexture(path, name);
            }
        }
    }

    bool Resources::LoadFont(const Path &inPath_, const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        auto fnt = Graphics::Font::LoadTTFFont(inPath_);
        if (fnt->IsValid()) {
            _Fonts.insert({ name_, std::unique_ptr<Graphics::Font>(fnt) });
            return true;
        }
        return false;
    }

    bool Resources::LoadMusic(const Path &inPath_, const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        auto mus = Audio::Music::LoadMusic(inPath_);
        if (mus->IsValid()) {
            _Music.insert({ name_, std::unique_ptr<Audio::Music>(mus) });
            return true;
        }
        return false;
    }

    bool Resources::LoadSound(const Path &inPath_, const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        auto snd = Audio::Sound::LoadSound(inPath_);
        if (snd->IsValid()) {
            _Sounds.insert({ name_, std::unique_ptr<Audio::Sound>(snd) });
            return true;
        }
        return false;
    }

    bool Resources::LoadTexture(const Path &inPath_, const std::string &name_) {
        name_ = std::regex_replace(name_, std::regex("\\\\"), "/");
        auto tex = Graphics::Texture2D::LoadTexture(inPath_);
        if (tex->IsValid()) {
            _Textures.insert({ name_, std::unique_ptr<Graphics::Texture2D>(tex) });
            return true;
        }
        return false;
    }
}
