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

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "../Config.hpp"

#include "../audio/Music.hpp"
#include "../audio/Sound.hpp"
#include "../graphics/Font.hpp"
#include "../graphics/GraphicsDevice.hpp"
#include "../graphics/Texture2D.hpp"
#include "Filesystem.hpp"

namespace ngine::filesystem {
    /**
     * Content type for content directories
     */
    enum ContentType {
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
        std::vector<std::pair<filesystem::Path, int>> ResourceDirectories =
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
        graphics::GraphicsDevice *m_graphicsDevice;

        /**
         * All named fonts.
         */
        std::unordered_map<std::string, graphics::Font *> m_fonts;

        /**
         * All named music.
         */
        std::unordered_map<std::string, audio::Music *> m_music;

        /**
         * All named sounds.
         */
        std::unordered_map<std::string, audio::Sound *> m_sounds;

        /**
         * All named textures.
         */
        std::unordered_map<std::string, graphics::Texture2D *> m_textures;
    public:
        /**
         * Create a new resource manager.
         *
         * @warning The `Game` class provides a resource manager. Using more than one resource manager could get confusing in your implementation.
         * @param graphicsDevice_ The graphics device for graphics resources.
         */
        ResourceManager(graphics::GraphicsDevice *graphicsDevice_);
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
        void deleteAll();

        /**
         * Delete a named font.
         *
         * @param name_ Font to be deleted.
         */
        void deleteFont(const std::string &name_);

        /**
         * Delete a named music.
         *
         * @param name_ Music to be deleted.
         */
        void deleteMusic(const std::string &name_);

        /**
         * Delete a named sound.
         *
         * @param name_ Sound to be deleted.
         */
        void deleteSound(const std::string &name_);

        /**
         * Delete a named texture.
         *
         * @param name_ Texture to be deleted.
         */
        void deleteTexture(const std::string &name_);

        /**
         * Get a named font.
         *
         * @param name_ Font to get.
         * @return Pointer to the font.
         */
        graphics::Font *getFont(const std::string &name_);

        /**
         * Get a named music.
         *
         * @param name_ Music to get.
         * @return Pointer to the music.
         */
        audio::Music *getMusic(const std::string &name_);

        /**
         * Get a named sound.
         *
         * @param name_ Sound to get.
         * @return Pointer to the sound.
         */
        audio::Sound *getSound(const std::string &name_);

        /**
         * Get a named texture.
         *
         * @param name_ Texture to get.
         * @return Pointer to the texture.
         */
        graphics::Texture2D *getTexture(const std::string &name_);

        /**
         * Loads all data according to the Config.
         */
        void loadResources();

        /**
         * Load font from file.
         *
         * @note If base size == -1, default will be used.
         * @param inPath_ Font file to load.
         * @param name_ The font's name.
         * @param baseSize_ The base size for the font.
         * @return Whether or not the font was loaded.
         */
        bool loadFont(const Path &inPath_, const std::string &name_, int baseSize_ = -1);

        /**
         * Load music from file.
         *
         * @param inPath_ Music file to load.
         * @param name_ The music's name.
         * @return Whether or not the music was loaded.
         */
        bool loadMusic(const Path &inPath_, const std::string &name_);

        /**
         * Load sound from file.
         *
         * @param inPath_ Sound file to load.
         * @param name_ The sound's name.
         * @return Whether or not the sound was loaded.
         */
        bool loadSound(const Path &inPath_, const std::string &name_);

        /**
         * Load texture from file.
         *
         * @param inPath_ Texture file to load.
         * @param name_ The texture's name.
         * @return Whether or not the texture was loaded.
         */
        bool loadTexture(const Path &inPath_, const std::string &name_);
    };
}

#endif //RESOURCEMANAGER_HPP
