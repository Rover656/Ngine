/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef RESOURCES_H
#define RESOURCES_H

#include "../Ngine.h"

#include "../Audio/Music.h"
#include "../Audio/Sound.h"
#include "../Graphics/Font.h"
#include "../Graphics/Texture2D.h"
#include "Filesystem.h"

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
     * Resource management.
     * This will control the loading, unloading and overall lifetime of loadable assets.
     */
    class NEAPI Resources {
        /**
         * All named fonts.
         */
        static std::unordered_map<std::string, std::unique_ptr<Graphics::Font>> _Fonts;

        /**
         * All named music.
         */
        static std::unordered_map<std::string, std::unique_ptr<Audio::Music>> _Music;

        /**
         * All named sounds.
         */
        static std::unordered_map<std::string, std::unique_ptr<Audio::Sound>> _Sounds;

        /**
         * All named textures.
         */
        static std::unordered_map<std::string, std::unique_ptr<Graphics::Texture2D>> _Textures;
    public:
        /**
         * The resource manager config.
         *
         * @note This should be configured *before* calling LoadResources().
         * @note To modify this, you should remember to DeleteAll() before calling LoadResources() again.
         */
        static ResourceManagerConfig Config;

        /**
         * Delete all resources.
         *
         * @warning This will break any pointers to existing textures, fonts etc. Ensure this is handled before calling.
         */
        static void DeleteAll();

        /**
         * Delete a named font.
         *
         * @param name_ Font to be deleted.
         */
        static void DeleteFont(const std::string &name_);

        /**
         * Delete a named music.
         *
         * @param name_ Music to be deleted.
         */
        static void DeleteMusic(const std::string &name_);

        /**
         * Delete a named sound.
         *
         * @param name_ Sound to be deleted.
         */
        static void DeleteSound(const std::string &name_);

        /**
         * Delete a named texture.
         *
         * @param name_ Texture to be deleted.
         */
        static void DeleteTexture(const std::string &name_);

        /**
         * Get a named font.
         *
         * @param name_ Font to get.
         * @return Pointer to the font.
         */
        static Graphics::Font *GetFont(const std::string &name_);

        /**
         * Get a named music.
         *
         * @param name_ Music to get.
         * @return Pointer to the music.
         */
        static Audio::Music *GetMusic(const std::string &name_);

        /**
         * Get a named sound.
         *
         * @param name_ Sound to get.
         * @return Pointer to the sound.
         */
        static Audio::Sound *GetSound(const std::string &name_);

        /**
         * Get a named texture.
         *
         * @param name_ Texture to get.
         * @return Pointer to the texture.
         */
        static Graphics::Texture2D *GetTexture(const std::string &name_);

        /**
         * Loads all data according to the Config.
         */
        static void LoadResources();

        /**
         * Load font from file.
         *
         * @note If base size == -1, default will be used.
         * @param inPath_ Font file to load.
         * @param name_ The font's name.
         * @param baseSize_ The base size for the font.
         * @return Whether or not the font was loaded.
         */
        static bool LoadFont(const Path &inPath_, const std::string &name_, int baseSize_ = -1);

        /**
         * Load music from file.
         *
         * @param inPath_ Music file to load.
         * @param name_ The music's name.
         * @return Whether or not the music was loaded.
         */
        static bool LoadMusic(const Path &inPath_, const std::string &name_);

        /**
         * Load sound from file.
         *
         * @param inPath_ Sound file to load.
         * @param name_ The sound's name.
         * @return Whether or not the sound was loaded.
         */
        static bool LoadSound(const Path &inPath_, const std::string &name_);

        /**
         * Load texture from file.
         *
         * @param inPath_ Texture file to load.
         * @param name_ The texture's name.
         * @return Whether or not the texture was loaded.
         */
        static bool LoadTexture(const Path &inPath_, const std::string &name_);
    };
}

#endif //RESOURCES_H
