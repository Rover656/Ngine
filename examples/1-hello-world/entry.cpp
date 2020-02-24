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

#include <Ngine.hpp>

class HelloWorldEntity : public ngine::Entity {
    ngine::graphics::Font *m_font = nullptr;
    ngine::Vector2 m_offset = ngine::Vector2::Zero;
public:
    void initialize(ngine::Scene *scene) {
        ngine::Entity::initialize(scene);
        m_font = getResourceManager()->getFont("Upheaval");

        // Calculate offset
        m_offset = m_font->measureString("Hello World!", 36, 1) / -2.0f;

        // TODO: This might still not be in the middle...
    }

    void draw(ngine::graphics::Renderer *renderer, ngine::Matrix modelView) {
        m_font->drawString(renderer, "Hello World!", m_offset, 36, 1, ngine::graphics::Color::Blue);
    }
};

class ExampleScene : public ngine::Scene {
    HelloWorldEntity *m_hello = nullptr;
public:
    ExampleScene() : Scene() {}

    void initialize(ngine::Game *game) override {
        // Add hello world entity in the center of the scene
        m_hello = new HelloWorldEntity();
        m_hello->setPosition({1280 / 2.0f, 768 / 2.0f});
        addChild(m_hello);
    }

    // TODO: Cleanup
};

class ExampleGame : public ngine::Game {
    ExampleScene *m_scene = nullptr;
public:
    ExampleGame(ngine::GameConfig gameConfig, ngine::WindowConfig windowConfig)
     : Game(windowConfig, gameConfig) {} // TODO: Swap fields in API

    void initialize() override {
        // Load all content (using default resource manager config).
        auto resMan = getResourceManager();
        resMan->loadResources();

        // Create scene
        m_scene = new ExampleScene();
        setScene(m_scene);
    }
};

NGINE_GAME_ENTRY {
    // Game config
    ngine::GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    gameConfig.MaintainResolution = true;

    // Window config
    ngine::WindowConfig windowConfig;
    windowConfig.Resizable = true;
    windowConfig.InitialWidth = 1280;
    windowConfig.InitialHeight = 768;

    // Create game
    auto game = ExampleGame(gameConfig, windowConfig);

    NGINE_RUN_GAME(game);
};