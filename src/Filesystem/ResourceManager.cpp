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

#include "ResourceManager.hpp"

#include <regex>
#include <sstream>

namespace NerdThings::Ngine::Filesystem {
    ResourceManager::ResourceManager(Graphics::GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {
        if (m_graphicsDevice == nullptr) throw std::runtime_error("Graphics device cannot be null.");
    }

    void ResourceManager::DeleteAll() {
        m_fonts.clear();
        m_music.clear();
        m_sounds.clear();
        m_textures.clear();
    }

    void ResourceManager::DeleteFont(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_fonts.find(name) != m_fonts.end()) {
            m_fonts.erase(name);
        }
    }

    void ResourceManager::DeleteMusic(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_music.find(name) != m_music.end()) {
            m_music.erase(name);
        }
    }

    void ResourceManager::DeleteSound(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_sounds.find(name) != m_sounds.end()) {
            m_sounds.erase(name);
        }
    }

    void ResourceManager::DeleteTexture(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_textures.find(name) != m_textures.end()) {
            m_textures.erase(name);
        }
    }

    Graphics::Font *ResourceManager::GetFont(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_fonts.find(name) != m_fonts.end())
            return m_fonts[name].get();
        return nullptr;
    }

    Audio::Music *ResourceManager::GetMusic(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_music.find(name) != m_music.end())
            return m_music[name].get();
        return nullptr;
    }

    Audio::Sound *ResourceManager::GetSound(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_sounds.find(name) != m_sounds.end())
            return m_sounds[name].get();
        return nullptr;
    }

    Graphics::Texture2D *ResourceManager::GetTexture(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_textures.find(name) != m_textures.end())
            return m_textures[name].get();
        return nullptr;
    }

    void ResourceManager::LoadResources() {
        // File extension definitions
        std::vector<std::string> fntExts = {"ttf", "otf"};
        std::vector<std::string> musExts = {"ogg", "flac", "mp3"};//, "xm", "mod"};
        std::vector<std::string> sndExts = {"wav", "ogg", "flac", "mp3"};
        std::vector<std::string> texExts = {"png", "bmp", "tga", "gif", "pic", "psd"};

        // Loop over all resource directories
        for (auto dir : Config.ResourceDirectories) {
            // TODO: Will this need to be more robust for some platforms?
            auto contentDir = Directory(dir.first.GetAbsolute());

            // Get all files
            auto files = contentDir.GetFilesRecursive();

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
                if (std::find(fntExts.begin(), fntExts.end(), ext) != fntExts.end() && dir.second & CONTENT_FONTS)
                    LoadFont(path, name);
                if (std::find(musExts.begin(), musExts.end(), ext) != musExts.end() && dir.second & CONTENT_MUSIC)
                    LoadMusic(path, name);
                if (std::find(sndExts.begin(), sndExts.end(), ext) != sndExts.end() && dir.second & CONTENT_SOUNDS)
                    LoadSound(path, name);
                if (std::find(texExts.begin(), texExts.end(), ext) != texExts.end() && dir.second & CONTENT_TEXTURES)
                    LoadTexture(path, name);
            }
        }
    }

    bool ResourceManager::LoadFont(const Path &inPath_, const std::string &name_, int baseSize_) {
        // Fix base size
        if (baseSize_ == -1) baseSize_ = Config.DefaultFontBaseSize;

        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create font
        auto fnt = Graphics::Font::LoadTTFFont(m_graphicsDevice, inPath_, baseSize_);

        // Check if it is valid
        if (fnt->IsValid()) {
            m_fonts.insert({name, std::unique_ptr<Graphics::Font>(fnt) });
            return true;
        }

        // Delete, it didnt work
        delete fnt;
        return false;
    }

    bool ResourceManager::LoadMusic(const Path &inPath_, const std::string &name_) {
        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create music
        auto mus = Audio::Music::LoadMusic(inPath_);

        // Check if it is valid
        if (mus->IsValid()) {
            m_music.insert({name, std::unique_ptr<Audio::Music>(mus) });
            return true;
        }

        // Delete, it didnt work
        delete mus;
        return false;
    }

    bool ResourceManager::LoadSound(const Path &inPath_, const std::string &name_) {
        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create sound
        auto snd = Audio::Sound::LoadSound(inPath_);

        // Check if it is valid
        if (snd->IsValid()) {
            m_sounds.insert({name, std::unique_ptr<Audio::Sound>(snd) });
            return true;
        }

        // Delete, it didnt work
        delete snd;
        return false;
    }

    bool ResourceManager::LoadTexture(const Path &inPath_, const std::string &name_) {
        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create texture
        auto tex = new Graphics::Texture2D(m_graphicsDevice, inPath_);

        // Check if it is valid.
        if (tex->IsValid()) {
            m_textures.insert({ name, std::unique_ptr<Graphics::Texture2D>(tex) });
            return true;
        }

        // Delete, it didnt work
        delete tex;
        return false;
    }
}
