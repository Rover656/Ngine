#include <Ngine.hpp>

#include <Physics/Shapes/CircleShape.hpp>
#include <Physics/Shapes/PolygonShape.hpp>

using namespace ngine;
using namespace ngine::audio;
using namespace ngine::components;
using namespace ngine::filesystem;
using namespace ngine::graphics;
using namespace ngine::input;
using namespace ngine::physics;
using namespace ngine::physics::shapes;

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 32;

    KeyboardMovementComponent2D(Entity *parent_) : Component(parent_) {
        subscribeToUpdate();
    }

    void update() override {
        auto par = getEntity<Entity>();
        Vector2 velocity;

        if (par->isPhysicsEnabled()) {
            velocity = par->getPhysicsWorld()->getGravityPixels();
        }

        if (Keyboard::GetCurrent()->isKeyDown(KEY_W)) {
            velocity.Y -= MoveSpeed;
        }

        if (Keyboard::GetCurrent()->isKeyDown(KEY_S)) {
            velocity.Y += MoveSpeed;
        }

        if (Keyboard::GetCurrent()->isKeyDown(KEY_A)) {
            velocity.X -= MoveSpeed;
        }

        if (Keyboard::GetCurrent()->isKeyDown(KEY_D)) {
            velocity.X += MoveSpeed;
        }

        if (par->isPhysicsEnabled()) {
            par->getPhysicsBody()->setLinearVelocity(velocity);
        } else {
            par->moveBy(velocity);
        }

        // Test

        if (Keyboard::GetCurrent()->isKeyDown(KEY_LEFT)) {
            par->setRotation(par->getRotation() - 5);
        }

        if (Keyboard::GetCurrent()->isKeyDown(KEY_RIGHT)) {
            auto rot = par->getRotation() + 5;
            auto rotDeg = rot.getDegrees();
            par->setRotation(par->getRotation() + 5);
        }

        if (Keyboard::GetCurrent()->isKeyDown(KEY_ZERO)) {
            par->setRotation(0);
        }
    }
};

class PlayerEntity : public Entity {
public:
    PlayerEntity(Vector2 position_)
            : Entity(position_, 0, 0, false, true) {
        OnInit += new ClassMethodEventHandler<PlayerEntity>(this, &PlayerEntity::Init);
    }

    void Init() {
        // Subscribe to updates.
        subscribeToUpdate();

        // Get physics body
        auto body = getPhysicsBody();

        // Set initial info
        PhysicsBody::BodyInfo bi;
        bi.Type = PhysicsBody::BODY_DYNAMIC;
        bi.Position = getPosition();
        bi.FixedRotation = true;
        body->set(bi);

        // Add fixtures (WIP)
        //auto shape = CircleShape(1);
        auto shape = PolygonShape();
        shape.setAsBox(16 / 16, 16 / 16);
        body->createFixture(&shape, 1);
        //auto shapeb = PolygonShape::CreateAsBox(16, 16);
        //body->CreateFixture(&shapeb, 1);

        // Apply test velocity
        //body->SetAngularVelocity(5);

        auto s = addComponent("Sprite", new SpriteComponent(this,
                                                            Sprite(getResourceManager()->getTexture("test_spritesheet"),
                                                                   16, 16, 32, 32, 30, 0)));
        s->setOrigin({16, 16});

        addComponent("Movement", new KeyboardMovementComponent2D(this));
        addComponent("Camera", new CameraComponent(this, 1, {1280 / 2.0f, 768 / 2.0f}))->activate();
    }

    void update() override {
        Entity::update();
        auto cam = getComponent<CameraComponent>("Camera");
        auto scene = getScene();

        auto w = scene->getViewportWidth();
        auto h = scene->getViewportHeight();
        cam->setOrigin({w / 2.0f, h / 2.0f});
    }
};

class Floor : public Entity {
public:
    Floor(Vector2 position_) : Entity(position_, 0, 0, false, true) {
        OnInit += new ClassMethodEventHandler<Floor>(this, &Floor::Init);
    }

    void Init() {
        // Get physics body
        auto body = getPhysicsBody();

        // Set initial body info
        PhysicsBody::BodyInfo bi;
        bi.Type = PhysicsBody::BODY_STATIC;
        bi.Position = getPosition();
        body->set(bi);

        // Add fixture
        auto box = PolygonShape::CreateAsBox(5, 5);
        body->createFixture(&box, 0);
    }
};

class TestScene : public Scene {
public:
    //TestWidget widg;

    TilesetRenderer *testTiles;

    TestScene(Game* game) : Scene(game, {0, 0}, 16) {//, widg(Vector2(120, 120)) {
        OnInit += new ClassMethodEventHandler<TestScene, SceneLoadEventArgs>(this, &TestScene::Init);
        OnUnload += new ClassMethodEventHandler<TestScene, SceneLoadEventArgs>(this, &TestScene::UnInit);
    }

    void Init(SceneLoadEventArgs e) {
        // Bind events
        OnDraw += new ClassMethodEventHandler<TestScene, graphics::Renderer *>(this, &TestScene::draw);
        OnUpdate += new ClassMethodEventHandler<TestScene>(this, &TestScene::update);
        OnDrawCamera += new ClassMethodEventHandler<TestScene, graphics::Renderer *>(this, &TestScene::DrawCam);

        // Add entities
        addChild("Player", new PlayerEntity({100, 100}));
        addChild("Floor", new Floor({100, 500}));

        // Configure physics
        getPhysicsWorld()->DebugDraw = true;

        // Setup cull area
        setCullArea(1280 - 50, 768 - 50, true);

        // Audio tests
        // ResourceManager::GetMusic("test_music")->Play();
        AudioDevice::SetMasterVolume(0.25);

        // Tileset tests
        std::vector<int> tileData;
        tileData.reserve(100);
        for (auto i = 0; i < 50; i++) {
            tileData.push_back(1);
        }
        for (auto i = 50; i < 100; i++) {
            tileData.push_back(2);
        }

        testTiles = new TilesetRenderer(Tileset(getResourceManager()->getTexture("test_tiles"), 32, 32), 10, 10, tileData);
        testTiles->setTileAt({5, 5}, 13);
    }

    void UnInit(SceneLoadEventArgs e) {
        // Clear events
        OnDrawCamera.clear();
        OnUpdate.clear();
        OnDrawCamera.clear();

        // Delete tilesets
        delete testTiles;
    }

    int rot = 0;

    void draw(graphics::Renderer *renderer_) {
        // ShapeRenderer::DrawCircle(renderer_, {50, 50}, 50, Color::Red, true, 1);
        // ShapeRenderer::DrawLine(renderer_, {10, 10}, {100, 100}, Color::Green, 5);
        // ShapeRenderer::DrawRectangle(renderer_, {10, 10, 100, 100}, Color::Red, 15, {});
        // ShapeRenderer::DrawRectangle(renderer_, {10, 10, 100, 100}, Color(0.0f, 1.0f, 0.0f, 0.5f), 15, {}, true, 15);
        // ShapeRenderer::DrawTriangle(renderer_, {40, 40}, {90, 90}, {40, 90}, Color::Orange, 0, {});
        // ShapeRenderer::DrawTriangle(renderer_, {40, 40}, {90, 90}, {40, 90}, Color(1.0f, 0.0f, 1.0f, 0.5f), 0, {}, true, 5);
        
        ShapeRenderer::DrawCircle(renderer_, Mouse::GetCurrent()->getMousePosition(), 15, Color::Blue);
    }

    void DrawCam(graphics::Renderer *renderer_) {
        //Renderer::DrawRectangleLines(GetCullArea(), Color::Green, 1);
        ShapeRenderer::DrawRectangle(renderer_, getCullArea(), Color::Green, 0, Vector2::Zero, true);
        testTiles->draw(renderer_, {0, 0}, getCullAreaPosition(), getCullAreaEndPosition(), 2.0f);

        auto text = "Hello World.\nGood day.";
        auto spacing = 0;
        auto fontSize = 32;
        auto lSpacing = LineSpacing::OnePointFive;
        auto rotation = 126;
        auto fnt = Font::GetDefaultFont();
        auto s = fnt->measureString(text, fontSize, spacing, lSpacing);
        ShapeRenderer::DrawRectangle(renderer_, {150, 150, s.X, s.Y}, Color::Green, rotation);
        fnt->drawString(renderer_, text, {150, 150}, fontSize, spacing, Color::Orange, lSpacing, rotation);
    }

    void update() {
        //widg.Update();

        if (Keyboard::GetCurrent()->isKeyPressed(KEY_F11)) {
            getGame()->getGameWindow()->toggleFullscreen();
        }
    }
};

class TestNode : public RenderableNode {
    Texture2D *m_texture;
public:
    TestNode(Texture2D *texture_) {
        m_texture = texture_;
    }

    void Render(Renderer *renderer_) override {
        //m_texture->Draw(renderer_, {0, 0});
        renderer_->setTexture(m_texture);
        renderer_->pushMatrix();
        renderer_->translate({0, 0, 0});

        renderer_->beginVertices(PrimitiveType::Quads);
        renderer_->pushVertex({
                                      {0, 0, 0},
                                      {0, 0},
                                      Color::White
                              });
        renderer_->pushVertex({
                                      {0, 64, 0},
                                      {0, 1},
                                      Color::White
                              });
        renderer_->pushVertex({
                                      {64, 64, 0},
                                      {1, 1},
                                      Color::White
                              });
        renderer_->pushVertex({
                                      {64, 0, 0},
                                      {1, 0},
                                      Color::White
                              });
        renderer_->endVertices();

        renderer_->popMatrix();

    }
};

#include <Graphics/ShaderProgramState.hpp>

class TestGame : public Game {
    TestScene *m_scene;

    RenderSpace *m_space;
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

        // Test new renderer capabilities
        //m_space->Render(renderer_, "defaultCamera");
    }

    void Init() {
        // Set exit key
        Keyboard::GetCurrent()->setExitKey(KEY_ESCAPE);

        // Load all content (using default resource manager config).
        auto resMan = getResourceManager();
        resMan->loadResources();

        // Load arial as default font
        Font::SetDefaultFont(resMan->getFont("Upheaval"));

        // Create test render space
        m_space = new RenderSpace();

        // Add a camera
        auto cam = m_space->createCamera("defaultCamera");
        //cam->Zoom = 2;

        // Add a node
        m_space->addNode(new TestNode(resMan->getTexture("test_spritesheet")));

        // Create scene
        m_scene = new TestScene(this);

        // Set scene
        setScene(m_scene);
    }
};

NGINE_GAME_ENTRY {
    // Load icon (must remain in scope until NGINE_RUN_GAME happens)
    graphics::Image icon(Path::GetExecutableDirectory() / "content" / "test_icon.png");

    // Set graphics API
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
    //GraphicsDevice::SetTargetAPI(GraphicsAPI::OpenGLES, 3, 1);
    GraphicsDevice::SetTargetAPI(GraphicsAPI::OpenGL, 4, 6);
#else
    GraphicsDevice::SetTargetAPI(GraphicsAPI::OpenGL, 3, 3);
#endif
#elif defined(PLATFORM_UWP)
    GraphicsDevice::SetTargetAPI(GraphicsAPI::OpenGLES, 2, 0);
#endif

    GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    gameConfig.RunWhileHidden = false; // For testing suspension.
    gameConfig.MaintainResolution = true; // TODO: Make it easier to manage when this is disabled.

    WindowConfig windowConfig;
    windowConfig.Resizable = true;
    //windowConfig.MSAA_4X = true;
    //windowConfig.VSync = true;
    windowConfig.InitialWidth = 1920/2;
    windowConfig.InitialHeight = 1080/2;
    windowConfig.Title = "Test Game";
    windowConfig.Icon = &icon;
    //windowConfig.NativeDebugConsole = true;

    // Create game
    auto game = TestGame(windowConfig, gameConfig);

    // Run game
    NGINE_RUN_GAME(game);

    return 0;
}