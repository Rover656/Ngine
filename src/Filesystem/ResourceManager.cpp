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

#include "filesystem/ResourceManager.hpp"

#include <regex>
#include <sstream>

namespace ngine::filesystem {
    ResourceManager::ResourceManager(graphics::GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {
        if (m_graphicsDevice == nullptr) throw std::runtime_error("Graphics device cannot be null.");
    }

    void ResourceManager::deleteAll() {
        // Delete all
        for (const auto& fnt : m_fonts) delete fnt.second;
        for (const auto& mus : m_music) delete mus.second;
        for (const auto& snd : m_sounds) delete snd.second;
        for (const auto& tex : m_textures) delete tex.second;

        // Clear
        m_fonts.clear();
        m_music.clear();
        m_sounds.clear();
        m_textures.clear();
    }

    void ResourceManager::deleteFont(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_fonts.find(name) != m_fonts.end()) {
            delete m_fonts[name];
            m_fonts.erase(name);
        }
    }

    void ResourceManager::deleteMusic(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_music.find(name) != m_music.end()) {
            delete m_music[name];
            m_music.erase(name);
        }
    }

    void ResourceManager::deleteSound(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_sounds.find(name) != m_sounds.end()) {
            delete m_sounds[name];
            m_sounds.erase(name);
        }
    }

    void ResourceManager::deleteTexture(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_textures.find(name) != m_textures.end()) {
            delete m_textures[name];
            m_textures.erase(name);
        }
    }

    graphics::Font *ResourceManager::getFont(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_fonts.find(name) != m_fonts.end())
            return m_fonts[name];
        return nullptr;
    }

    audio::Music *ResourceManager::getMusic(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_music.find(name) != m_music.end())
            return m_music[name];
        return nullptr;
    }

    audio::Sound *ResourceManager::getSound(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_sounds.find(name) != m_sounds.end())
            return m_sounds[name];
        return nullptr;
    }

    graphics::Texture2D *ResourceManager::getTexture(const std::string &name_) {
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");
        if (m_textures.find(name) != m_textures.end())
            return m_textures[name];
        return nullptr;
    }

    void ResourceManager::loadResources() {
        // File extension definitions
        std::vector<std::string> fntExts = {"ttf", "otf"};
        std::vector<std::string> musExts = {"ogg", "flac", "mp3"};//, "xm", "mod"};
        std::vector<std::string> sndExts = {"wav", "ogg", "flac", "mp3"};
        std::vector<std::string> texExts = {"png", "bmp", "tga", "gif", "pic", "psd"};

        // Loop over all resource directories
        for (auto dir : Config.ResourceDirectories) {
            // TODO: Will this need to be more robust for some platforms?
            auto contentDir = Directory(dir.first.getAbsolute());

            // Get all files
            auto files = contentDir.getFilesRecursive();

            // Load all files
            for (auto file : files) {
                // Get name (relative path to the content folder)
                auto name = file.getPath().getRelativeTo(contentDir.getPath()).getStringNoExtension();

                // Replace windows slashes with forward slashes
                name = std::regex_replace(name, std::regex("\\\\"), "/");

                // Get extension
                auto ext = file.getFileExtension();

                // Get actual path
                auto path = file.getPath();

                // Load resources
                if (std::find(fntExts.begin(), fntExts.end(), ext) != fntExts.end() && dir.second & CONTENT_FONTS)
                    loadFont(path, name);
                if (std::find(musExts.begin(), musExts.end(), ext) != musExts.end() && dir.second & CONTENT_MUSIC)
                    loadMusic(path, name);
                if (std::find(sndExts.begin(), sndExts.end(), ext) != sndExts.end() && dir.second & CONTENT_SOUNDS)
                    loadSound(path, name);
                if (std::find(texExts.begin(), texExts.end(), ext) != texExts.end() && dir.second & CONTENT_TEXTURES)
                    loadTexture(path, name);
            }
        }
    }

    bool ResourceManager::loadFont(const Path &inPath_, const std::string &name_, int baseSize_) {
        // Fix base size
        if (baseSize_ == -1) baseSize_ = Config.DefaultFontBaseSize;

        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create font
        auto fnt = graphics::Font::LoadTTFFont(m_graphicsDevice, inPath_, baseSize_);

        // Check if it is valid
        if (fnt->isValid()) {
            m_fonts.insert({name, fnt});
            return true;
        }

        // Delete, it didnt work
        delete fnt;
        return false;
    }

    bool ResourceManager::loadMusic(const Path &inPath_, const std::string &name_) {
        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create music
        auto mus = audio::Music::LoadMusic(inPath_);

        // Check if it is valid
        if (mus->isValid()) {
            m_music.insert({name, mus});
            return true;
        }

        // Delete, it didnt work
        delete mus;
        return false;
    }

    bool ResourceManager::loadSound(const Path &inPath_, const std::string &name_) {
        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create sound
        auto snd = audio::Sound::LoadSound(inPath_);

        // Check if it is valid
        if (snd->isValid()) {
            m_sounds.insert({name, snd});
            return true;
        }

        // Delete, it didnt work
        delete snd;
        return false;
    }

    bool ResourceManager::loadTexture(const Path &inPath_, const std::string &name_) {
        // Get the name
        auto name = std::regex_replace(name_, std::regex("\\\\"), "/");

        // Create texture
        auto tex = graphics::Texture2D::LoadTexture(m_graphicsDevice, inPath_);

        // Check if it is valid.
        if (tex->isValid()) {
            m_textures.insert({ name, tex});
            return true;
        }

        // Delete, it didnt work
        delete tex;
        return false;
    }
}
