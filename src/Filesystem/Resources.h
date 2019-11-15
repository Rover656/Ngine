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
    /*
     * Content type for content directories
     */
    enum ResourceDirectoryContentType {
        CONTENT_NONE = 0,
        CONTENT_TEXTURES = 1,
        CONTENT_SOUNDS = 2,
        CONTENT_MUSIC = 4,
        CONTENT_FONTS = 8, // TODO: Custom type support
    };

    struct ResourceManagerConfig {
        /*
         * Default base size for loaded fonts.
         * Default: 36
         */
        int DefaultFontBaseSize = 36;

        /*
         * The directory structure of the resources directory.
         */
        std::vector<std::pair<Filesystem::Path, int>> ResourceDirectories = {
                {
                    Path("content"),
                    CONTENT_TEXTURES | CONTENT_SOUNDS | CONTENT_MUSIC | CONTENT_FONTS
                }
        };
    };

    /*
     * Resource management class
     */
    class NEAPI Resources {
        // Private Fields

        static std::unordered_map<std::string, std::unique_ptr<Graphics::Font>> _Fonts;

        /*
         * All named music
         */
        static std::unordered_map<std::string, std::unique_ptr<Audio::Music>> _Music;

        /*
         * All named sounds
         */
        static std::unordered_map<std::string, std::unique_ptr<Audio::Sound>> _Sounds;

        /*
         * All named textures
         */
        static std::unordered_map<std::string, std::unique_ptr<Graphics::Texture2D>> _Textures;
    public:

        // Public Fields

        /*
         * The resource manager config
         */
        static ResourceManagerConfig Config;

        // Public Methods

        /*
         * Delete all resources
         */
        static void DeleteAll();

        /*
         * Delete a named font
         */
        static void DeleteFont(const std::string &name_);

        /*
         * Delete a named music
         */
        static void DeleteMusic(const std::string &name_);

        /*
         * Delete a named sound
         */
        static void DeleteSound(const std::string &name_);

        /*
         * Delete a named texture
         */
        static void DeleteTexture(const std::string &name_);

        /*
         * Get a named font
         */
        static Graphics::Font *GetFont(const std::string &name_);

        /*
         * Get a named music
         */
        static Audio::Music *GetMusic(const std::string &name_);

        /*
         * Get a named sound
         */
        static Audio::Sound *GetSound(const std::string &name_);

        /*
         * Get a named texture
         */
        static Graphics::Texture2D *GetTexture(const std::string &name_);

        /*
         * Loads all files in the resources directory.
         * All names will be set to their relative path without their extension
         */
        static void LoadResources();

        /*
         * Load font from file.
         * If base size == -1, default will be used.
         */
        static bool LoadFont(const Path &inPath_, const std::string &name_, int baseSize_ = -1);

        /*
         * Load music from file
         */
        static bool LoadMusic(const Path &inPath_, const std::string &name_);

        /*
         * Load sound from file
         */
        static bool LoadSound(const Path &inPath_, const std::string &name_);

        /*
         * Load texture from file
         */
        static bool LoadTexture(const Path &inPath_, const std::string &name_);
    };
}

#endif //RESOURCES_H
