#include <Ngine.hpp>

using namespace ngine;
using namespace ngine::audio;
//using namespace ngine::components;
using namespace ngine::filesystem;
using namespace ngine::graphics;
using namespace ngine::input;
//using namespace ngine::physics;

class SpriteComponentTest : public Component {
    Sprite *m_sprite;

public:
    SpriteComponentTest(Sprite *sprite)
            : m_sprite(sprite) {}

    void draw(Renderer *renderer) override {
        m_sprite->draw(renderer, {0, 0});
    }
};

class TestEntity : public Entity {
public:
    TestEntity() {}

    void initialize(Scene *scene) override {
        // Call to base first.
        Entity::initialize(scene);

        // Set defaults
        setPosition({0, 0});
        setRotation(0);
        setScale({2, 2});

        // Set name
        setName("TestEntity");

        // Add component.
        auto sprite = new Sprite(getResourceManager()->getTexture("test_spritesheet"), 16, 16, 32, 32);
        addComponent("Sprite", new SpriteComponentTest(sprite));
    }

    void update() override {
        Entity::update();

        auto keyboard = Keyboard::GetCurrent();
        Vector2 vel;

        auto ms = 5;

        if (keyboard->isKeyDown(Key::KEY_W)) {
            vel.Y -= ms;
        }

        if (keyboard->isKeyDown(Key::KEY_S)) {
            vel.Y += ms;
        }

        if (keyboard->isKeyDown(Key::KEY_A)) {
            vel.X -= ms;
        }

        if (keyboard->isKeyDown(Key::KEY_D)) {
            vel.X += ms;
        }

        auto rot = getRotation();

        if (keyboard->isKeyDown(Key::KEY_LEFT)) {
            rot = rot + 5;
        }

        if (keyboard->isKeyDown(Key::KEY_RIGHT)) {
            rot = rot - 5;
        }

        auto scale = getScale();

        if (keyboard->isKeyPressed(Key::KEY_UP)) {
            scale.X += 0.25;
            scale.Y += 0.25;
        }

        if (keyboard->isKeyPressed(Key::KEY_DOWN)) {
            scale.X -= 0.25;
            scale.Y -= 0.25;
        }

        // Move
        auto pos = getPosition();
        setPosition(pos + vel);
        setRotation(rot);
        setScale(scale);

        if (keyboard->isKeyPressed(Key::KEY_SPACE)) {
            setPosition({0, 0});
        }

        if (keyboard->isKeyPressed(Key::KEY_ZERO)) {
            setRotation(0);
        }
    }
};

class ParentEntity : public Entity {
public:
    void initialize(Scene *scene) override {
        Entity::initialize(scene);
        setPosition({150, 150});
        setRotation(45);
    }

    void draw(Renderer *renderer, Matrix modelView) override {
        Entity::draw(renderer, modelView);
        ShapeRenderer::DrawCircle(renderer, {0, 0}, 15, Color::Blue);
    }
};

// TESTING
class NewTestScene : public Scene {
public:
    NewTestScene(Game *game) : Scene(game) {
//        auto par = new ParentEntity();
//        par->addChild(new TestEntity());
//        addChild(par);
        addChild(new TestEntity());
    }
};

class TestGame : public Game {
    NewTestScene *m_scene;
public:

    TestGame(const WindowConfig &windowConfig_, const GameConfig &config_) : Game(windowConfig_, config_) {
        // Hook events
        OnInit += new ClassMethodEventHandler<TestGame>(this, &TestGame::Init);
        OnDraw += new ClassMethodEventHandler<TestGame, graphics::Renderer *>(this, &TestGame::Draw);
        OnSuspend += new ClassMethodEventHandler<TestGame>(this, &TestGame::Save);
        OnResume += new ClassMethodEventHandler<TestGame>(this, &TestGame::Load);
    }

    void Save() {
        Console::Notice("TestGame", "The game would have saved here.");
    }

    void Load() {
        Console::Notice("TestGame", "The game would have loaded here.");
    }

    void Draw(graphics::Renderer *renderer_) {
        // ShapeRenderer::DrawTriangle(renderer_, {40, 40}, {90, 90}, {40, 90}, Color::Orange, 0, {});
        ShapeRenderer::DrawRectangle(renderer_, getGameViewport(), Color::Blue, 0, {}, true, 5);

        ShapeRenderer::DrawCircle(renderer_, {150, 150}, 2, Color::Red);
    }

    void Init() {
        // Set exit key
        Keyboard::GetCurrent()->setExitKey(KEY_ESCAPE);

        // Load all content (using default resource manager config).
        auto resMan = getResourceManager();
        resMan->loadResources();

        // Load arial as default font
        Font::SetDefaultFont(resMan->getFont("Upheaval"));

        // Create and enable scene.
        m_scene = new NewTestScene(this);
        setScene(m_scene);
    }
};

NGINE_GAME_ENTRY {
    // Load icon (must remain in scope until NGINE_RUN_GAME happens)
    graphics::Image icon(Path::GetExecutableDirectory() / "content" / "test_icon.png");

    GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    gameConfig.RunWhileHidden = false; // For testing suspension.
    gameConfig.FPSCap = 60;
    // gameConfig.MaintainResolution = true; // TODO: Make it easier to manage when this is disabled.

    WindowConfig windowConfig;
    windowConfig.Resizable = true;
    windowConfig.MSAA_4X = true;
    windowConfig.VSync = true;
    windowConfig.InitialWidth = 1920 / 2;
    windowConfig.InitialHeight = 1080 / 2;
    windowConfig.Title = "Test Game";
    windowConfig.Icon = &icon;
    //windowConfig.NativeDebugConsole = true;

#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
    windowConfig.TargetAPI = GraphicsAPI::OpenGL;
    windowConfig.TargetAPIMajorVersion = 4;
    windowConfig.TargetAPIMinorVersion = 6;
#endif

    // Create game
    auto game = TestGame(windowConfig, gameConfig);

    // Run game
    NGINE_RUN_GAME(game);

    return 0;
}