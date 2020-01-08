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

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "../Config.hpp"

#include "../Audio/Music.hpp"
#include "../Audio/Sound.hpp"
#include "../Graphics/Font.hpp"
#include "../Graphics/GraphicsDevice.hpp"
#include "../Graphics/Texture2D.hpp"
#include "Filesystem.hpp"

namespace NerdThings::Ngine::Filesystem {
    /**
     * Content type for content directories
     */
    enum ResourceDirectoryContentType {
        CONTENT_NONE = 0,
        CONTENT_TEXTURES = 1,
        CONTENT_SOUNDS = 2,
        CONTENT_MUSIC = 4,
        CONTENT_FONTS = 8, // TODO: Custom type support
    };

    /**
     * Resource manager configuration.
     * This details exactly what resources should be loaded from where.
     * By default this recursively loads *everything* from the 'content' directory.
     */
    struct ResourceManagerConfig {
        /**
         * Default base size for loaded fonts.
         * Default: 36.
         *
         * @warning The higher this value is set, the more memory each loaded font will use.
         * @warning This will also affect how well fonts can be scaled up or down.
         */
        int DefaultFontBaseSize = 36;

        /**
         * The directory structure of the resources directory.
         */
        std::vector<std::pair<Filesystem::Path, int>> ResourceDirectories =
            {
                {
                    Path("content"),
                    CONTENT_TEXTURES | CONTENT_SOUNDS | CONTENT_MUSIC | CONTENT_FONTS
                }
            };
    };

    /**
     * Resource management helper.
     * This will control the loading, unloading and overall lifetime of loadable assets.
     */
    class NEAPI ResourceManager {
        /**
         * The graphics device we use to create graphics resources.
         */
        Graphics::GraphicsDevice *m_graphicsDevice;

        /**
         * All named fonts.
         */
        std::unordered_map<std::string, Graphics::Font *> m_fonts;

        /**
         * All named music.
         */
        std::unordered_map<std::string, Audio::Music *> m_music;

        /**
         * All named sounds.
         */
        std::unordered_map<std::string, Audio::Sound *> m_sounds;

        /**
         * All named textures.
         */
        std::unordered_map<std::string, Graphics::Texture2D *> m_textures;
    public:
        /**
         * Create a new resource manager.
         *
         * @warning The `Game` class provides a resource manager. Using more than one resource manager could get confusing in your implementation.
         * @param graphicsDevice_ The graphics device for graphics resources.
         */
        ResourceManager(Graphics::GraphicsDevice *graphicsDevice_);
        ~ResourceManager() = default;
        /**
         * The resource manager config.
         *
         * @note This should be configured *before* calling LoadResources().
         * @note To modify this, you should remember to DeleteAll() before calling LoadResources() again.
         */
        ResourceManagerConfig Config;

        /**
         * Delete all resources.
         *
         * @warning This will break any pointers to existing textures, fonts etc. Ensure this is handled before calling.
         */
        void DeleteAll();

        /**
         * Delete a named font.
         *
         * @param name_ Font to be deleted.
         */
        void DeleteFont(const std::string &name_);

        /**
         * Delete a named music.
         *
         * @param name_ Music to be deleted.
         */
        void DeleteMusic(const std::string &name_);

        /**
         * Delete a named sound.
         *
         * @param name_ Sound to be deleted.
         */
        void DeleteSound(const std::string &name_);

        /**
         * Delete a named texture.
         *
         * @param name_ Texture to be deleted.
         */
        void DeleteTexture(const std::string &name_);

        /**
         * Get a named font.
         *
         * @param name_ Font to get.
         * @return Pointer to the font.
         */
        Graphics::Font *GetFont(const std::string &name_);

        /**
         * Get a named music.
         *
         * @param name_ Music to get.
         * @return Pointer to the music.
         */
        Audio::Music *GetMusic(const std::string &name_);

        /**
         * Get a named sound.
         *
         * @param name_ Sound to get.
         * @return Pointer to the sound.
         */
        Audio::Sound *GetSound(const std::string &name_);

        /**
         * Get a named texture.
         *
         * @param name_ Texture to get.
         * @return Pointer to the texture.
         */
        Graphics::Texture2D *GetTexture(const std::string &name_);

        /**
         * Loads all data according to the Config.
         */
        void LoadResources();

        /**
         * Load font from file.
         *
         * @note If base size == -1, default will be used.
         * @param inPath_ Font file to load.
         * @param name_ The font's name.
         * @param baseSize_ The base size for the font.
         * @return Whether or not the font was loaded.
         */
        bool LoadFont(const Path &inPath_, const std::string &name_, int baseSize_ = -1);

        /**
         * Load music from file.
         *
         * @param inPath_ Music file to load.
         * @param name_ The music's name.
         * @return Whether or not the music was loaded.
         */
        bool LoadMusic(const Path &inPath_, const std::string &name_);

        /**
         * Load sound from file.
         *
         * @param inPath_ Sound file to load.
         * @param name_ The sound's name.
         * @return Whether or not the sound was loaded.
         */
        bool LoadSound(const Path &inPath_, const std::string &name_);

        /**
         * Load texture from file.
         *
         * @param inPath_ Texture file to load.
         * @param name_ The texture's name.
         * @return Whether or not the texture was loaded.
         */
        bool LoadTexture(const Path &inPath_, const std::string &name_);
    };
}

#endif //RESOURCEMANAGER_HPP
