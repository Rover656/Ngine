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

#include <sstream>

namespace NerdThings::Ngine::Filesystem {
    // Private Fields

    std::unordered_map<std::string, Graphics::TFont> Resources::_Fonts;
    std::unordered_map<std::string, Audio::TMusic> Resources::_Music;
    std::unordered_map<std::string, Audio::TSound> Resources::_Sounds;
    std::unordered_map<std::string, Graphics::TTexture2D> Resources::_Textures;

    // Public Fields

    TPath Resources::ResourcesDirectory = TPath("content");

    // Public Methods

    void Resources::DeleteAll() {
        _Fonts.clear();
        _Music.clear();
        _Sounds.clear();
        _Textures.clear();
    }

    void Resources::DeleteFont(const std::string &name_) {
        if (_Fonts.find(name_) != _Fonts.end()) {
            _Fonts.erase(name_);
        }
    }

    void Resources::DeleteMusic(const std::string &name_) {
        if (_Music.find(name_) != _Music.end()) {
            _Music.erase(name_);
        }
    }

    void Resources::DeleteSound(const std::string &name_) {
        if (_Sounds.find(name_) != _Sounds.end()) {
            _Sounds.erase(name_);
        }
    }

    void Resources::DeleteTexture(const std::string &name_) {
        if (_Textures.find(name_) != _Textures.end()) {
            _Textures.erase(name_);
        }
    }

    Graphics::TFont Resources::GetFont(const std::string &name_) {
        if (_Fonts.find(name_) != _Fonts.end())
            return _Fonts[name_];
        return Graphics::TFont();
    }

    Audio::TMusic Resources::GetMusic(const std::string &name_) {
        if (_Music.find(name_) != _Music.end())
            return _Music[name_];
        return Audio::TMusic();
    }

    Audio::TSound Resources::GetSound(const std::string &name_) {
        if (_Sounds.find(name_) != _Sounds.end())
            return _Sounds[name_];
        return Audio::TSound();
    }

    Graphics::TTexture2D Resources::GetTexture(const std::string &name_) {
        if (_Textures.find(name_) != _Textures.end())
            return _Textures[name_];
        return Graphics::TTexture2D();
    }

    std::string Resources::GetWorkingDirectory() {
        return std::filesystem::current_path().string();
    }

    void Resources::LoadResources() {
        // Get content dir
        auto contentDir = TDirectory(TPath(TPath::GetExecutableDirectory(), ResourcesDirectory));

        // Get all files
        auto files = contentDir.GetFilesRecursive();

        // File extension definitions
        // TODO: Align this with whatever spec we will support
        std::vector<std::string> fntExts = {"ttf", "otf", "fnt"}; // TODO: Spritefont support

        std::vector<std::string> musExts = {"ogg", "flac", "mp3", "xm", "mod"};
        std::vector<std::string> sndExts = {"wav", "ogg", "flac", "mp3"};
        std::vector<std::string> texExts = {"png", "bmp", "tga", "gif", "pic", "psd", "hdr", "dds", "pkm", "ktx", "pvr", "astc"};

        // Load all files
        for (auto file : files) {
            auto name = file.GetObjectName();
            auto ext = file.GetFileExtension();
            auto path = file.GetObjectPath();

            if (std::find(fntExts.begin(), fntExts.end(), ext) != fntExts.end()) { // Font
                LoadFont(path, name);
            } else if (std::find(musExts.begin(), musExts.end(), ext) != musExts.end()) { // Music
                LoadMusic(path, name);
            } else if (std::find(sndExts.begin(), sndExts.end(), ext) != sndExts.end()) { // Sound
                LoadSound(path, name);
            } else if (std::find(texExts.begin(), texExts.end(), ext) != texExts.end()) { // Texture
                LoadTexture(path, name);
            }
        }
    }

    bool Resources::LoadFont(const TPath &inPath_, const std::string &name_) {
//        auto fnt = Graphics::TFont::LoadFont(inPath_);
//        if (fnt->Texture->ID > 0) {
//            _Fonts.insert({ name_, fnt });
//            return true;
//        }
//        return false;
        return false;
    }

    bool Resources::LoadMusic(const TPath &inPath_, const std::string &name_) {
//        auto mus = Audio::TMusic::LoadMusic(inPath_);
//        if (mus->MusicData != nullptr) {
//            _Music.insert({ name_, mus });
//            return true;
//        }
//        return false;
        return false;
    }

    bool Resources::LoadSound(const TPath &inPath_, const std::string &name_) {
//        auto snd = Audio::TSound::LoadSound(inPath_);
//        if (snd->AudioBuffer != nullptr) {
//            _Sounds.insert({ name_, snd });
//            return true;
//        }
//        return false;
        return false;
    }

    bool Resources::LoadTexture(const TPath &inPath_, const std::string &name_) {
        auto tex = Graphics::TTexture2D::LoadTexture(inPath_);
        if (tex.IsValid()) {
            _Textures.insert({ name_, tex });
            return true;
        }
        return false;
    }
}
