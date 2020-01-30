#include <Ngine.hpp>

#include <Physics/Shapes/CircleShape.hpp>
#include <Physics/Shapes/PolygonShape.hpp>

using namespace Ngine;
using namespace Ngine::Audio;
using namespace Ngine::Components;
using namespace Ngine::Filesystem;
using namespace Ngine::Graphics;
using namespace Ngine::Input;
using namespace Ngine::Physics;
using namespace Ngine::Physics::Shapes;

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 32;

    KeyboardMovementComponent2D(Entity *parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update() override {
        auto par = GetEntity < Entity > ();
        Vector2 velocity;

        if (par->IsPhysicsEnabled()) {
            velocity = par->GetPhysicsWorld()->GetGravityPixels();
        }

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_W)) {
            velocity.Y -= MoveSpeed;
        }

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_S)) {
            velocity.Y += MoveSpeed;
        }

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_A)) {
            velocity.X -= MoveSpeed;
        }

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_D)) {
            velocity.X += MoveSpeed;
        }

        if (par->IsPhysicsEnabled()) {
            par->GetPhysicsBody()->SetLinearVelocity(velocity);
        } else {
            par->MoveBy(velocity);
        }

        // Test

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_LEFT)) {
            par->SetRotation(par->GetRotation() - 5);
        }

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_RIGHT)) {
            auto rot = par->GetRotation() + 5;
            auto rotDeg = rot.GetDegrees();
            par->SetRotation(par->GetRotation() + 5);
        }

        if (Keyboard::GetCurrent()->IsKeyDown(KEY_ZERO)) {
            par->SetRotation(0);
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
        SubscribeToUpdate();

        // Get physics body
        auto body = GetPhysicsBody();

        // Set initial info
        PhysicsBody::BodyInfo bi;
        bi.Type = PhysicsBody::BODY_DYNAMIC;
        bi.Position = GetPosition();
        bi.FixedRotation = true;
        body->Set(bi);

        // Add fixtures (WIP)
        //auto shape = CircleShape(1);
        auto shape = PolygonShape();
        shape.SetAsBox(16 / 16, 16 / 16);
        body->CreateFixture(&shape, 1);
        //auto shapeb = PolygonShape::CreateAsBox(16, 16);
        //body->CreateFixture(&shapeb, 1);

        // Apply test velocity
        //body->SetAngularVelocity(5);

        auto s = AddComponent("Sprite", new SpriteComponent(this, Sprite(GetResourceManager()->GetTexture("test_spritesheet"), 16, 16, 32, 32, 30, 0)));
        s->SetOrigin({16, 16});

        AddComponent("Movement", new KeyboardMovementComponent2D(this));
        AddComponent("Camera", new CameraComponent(this, 1, {1280/2.0f, 768/2.0f}))->Activate();
    }

    void Update() override {
        Entity::Update();
        auto cam = GetComponent<CameraComponent>("Camera");
        auto scene = GetScene();

        auto w = scene->GetViewportWidth();
        auto h = scene->GetViewportHeight();
        cam->SetOrigin({w / 2.0f, h / 2.0f});
    }
};

class Floor : public Entity {
public:
    Floor(Vector2 position_) : Entity(position_, 0, 0, false, true) {
        OnInit += new ClassMethodEventHandler<Floor>(this, &Floor::Init);
    }

    void Init() {
        // Get physics body
        auto body = GetPhysicsBody();

        // Set initial body info
        PhysicsBody::BodyInfo bi;
        bi.Type = PhysicsBody::BODY_STATIC;
        bi.Position = GetPosition();
        body->Set(bi);

        // Add fixture
        auto box = PolygonShape::CreateAsBox(5, 5);
        body->CreateFixture(&box, 0);
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
        OnDraw += new ClassMethodEventHandler<TestScene, Graphics::Renderer *>(this, &TestScene::Draw);
        OnUpdate += new ClassMethodEventHandler<TestScene>(this, &TestScene::Update);
        OnDrawCamera += new ClassMethodEventHandler<TestScene, Graphics::Renderer *>(this, &TestScene::DrawCam);

        // Add entities
        AddChild("Player", new PlayerEntity({100, 100}));
        AddChild("Floor", new Floor({100, 500}));

        // Configure physics
        GetPhysicsWorld()->DebugDraw = true;

        // Setup cull area
        SetCullArea(1280 - 50, 768 - 50, true);

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

        testTiles = new TilesetRenderer(Tileset(GetResourceManager()->GetTexture("test_tiles"), 32, 32), 10, 10, tileData);
        testTiles->SetTileAt({5, 5}, 13);
    }

    void UnInit(SceneLoadEventArgs e) {
        // Clear events
        OnDrawCamera.Clear();
        OnUpdate.Clear();
        OnDrawCamera.Clear();

        // Delete tilesets
        delete testTiles;
    }

    int rot = 0;

    void Draw(Graphics::Renderer *renderer_) {
        // ShapeRenderer::DrawCircle(renderer_, {50, 50}, 50, Color::Red, true, 1);
        // ShapeRenderer::DrawLine(renderer_, {10, 10}, {100, 100}, Color::Green, 5);
        // ShapeRenderer::DrawRectangle(renderer_, {10, 10, 100, 100}, Color::Red, 15, {});
        // ShapeRenderer::DrawRectangle(renderer_, {10, 10, 100, 100}, Color(0.0f, 1.0f, 0.0f, 0.5f), 15, {}, true, 15);
        // ShapeRenderer::DrawTriangle(renderer_, {40, 40}, {90, 90}, {40, 90}, Color::Orange, 0, {});
        // ShapeRenderer::DrawTriangle(renderer_, {40, 40}, {90, 90}, {40, 90}, Color(1.0f, 0.0f, 1.0f, 0.5f), 0, {}, true, 5);
        
        ShapeRenderer::DrawCircle(renderer_, Mouse::GetCurrent()->GetMousePosition(), 15, Color::Blue);
    }

    void DrawCam(Graphics::Renderer *renderer_) {
        //Renderer::DrawRectangleLines(GetCullArea(), Color::Green, 1);
        ShapeRenderer::DrawRectangle(renderer_, GetCullArea(), Color::Green, 0, Vector2::Zero, true);
        testTiles->Draw(renderer_, {0, 0}, GetCullAreaPosition(), GetCullAreaEndPosition(), 2.0f);

        auto text = "Hello World.\nGood day.";
        auto spacing = 0;
        auto fontSize = 32;
        auto lSpacing = LineSpacing::OnePointFive;
        auto rotation = 126;
        auto fnt = Font::GetDefaultFont();
        auto s = fnt->MeasureString(text, fontSize, spacing, lSpacing);
        ShapeRenderer::DrawRectangle(renderer_, {150, 150, s.X, s.Y}, Color::Green, rotation);
        fnt->DrawString(renderer_, text, {150, 150}, fontSize, spacing, Color::Orange, lSpacing, rotation);
    }

    void Update() {
        //widg.Update();

        if (Keyboard::GetCurrent()->IsKeyPressed(KEY_F11)) {
            GetGame()->GetGameWindow()->ToggleFullscreen();
        }
    }
};

class TestNode : public RenderableNode {
    Texture2D *m_texture;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned short> m_indices;
public:
    TestNode(Texture2D *texture_) {
        m_texture = texture_;

        VertexDataTool::GenerateRect(100, 100, Color::White, m_vertices, m_indices);
    }

    void Render(Renderer *renderer_) override {
        //m_texture->Draw(renderer_, {0, 0});
        renderer_->SetTexture(m_texture);
        renderer_->PushMatrix();
        renderer_->Translate({0, 0, 0});
//        renderer_->AddIndexedTriangles(m_vertices.data(), m_vertices.size(), m_indices.data(), m_indices.size());

        renderer_->BeginVertices(PrimitiveType::Quads);
        renderer_->PushVertex({
                                      {0, 0, 0},
                                      {0, 0},
                                      Color::White
        });
        renderer_->PushVertex({
                                      {0, 64, 0},
                                      {0, 1},
                                      Color::White
                              });
        renderer_->PushVertex({
                                      {64, 64, 0},
                                      {1, 1},
                                      Color::White
                              });
        renderer_->PushVertex({
                                      {64, 0, 0},
                                      {1, 0},
                                      Color::White
                              });
        renderer_->EndVertices();

        renderer_->PopMatrix();

    }
};

class TestGame : public Game {
    TestScene *m_scene;

    RenderSpace *m_space;
public:

    TestGame(const WindowConfig &windowConfig_, const GameConfig &config_) : Game(windowConfig_, config_) {
        // Hook events
        OnInit += new ClassMethodEventHandler<TestGame>(this, &TestGame::Init);
        OnDraw += new ClassMethodEventHandler<TestGame, Graphics::Renderer *>(this, &TestGame::Draw);
        OnSuspend += new ClassMethodEventHandler<TestGame>(this, &TestGame::Save);
        OnResume += new ClassMethodEventHandler<TestGame>(this, &TestGame::Load);
    }

    void Save() {
        Console::Notice("TestGame", "The game would have saved here.");
    }

    void Load() {
        Console::Notice("TestGame", "The game would have loaded here.");
    }

    void Draw(Graphics::Renderer *renderer_) {
        // ShapeRenderer::DrawTriangle(renderer_, {40, 40}, {90, 90}, {40, 90}, Color::Orange, 0, {});

        // Test new renderer capabilities
        //m_space->Render(renderer_, "defaultCamera");
    }

    void Init() {
        // Set exit key
        Keyboard::GetCurrent()->SetExitKey(KEY_ESCAPE);

        // Load all content (using default resource manager config).
        auto resMan = GetResourceManager();
        resMan->LoadResources();

        // Load arial as default font
        Font::SetDefaultFont(resMan->GetFont("Upheaval"));

        // Create test render space
        m_space = new RenderSpace();

        // Add a camera
        auto cam = m_space->CreateCamera("defaultCamera");
        //cam->Zoom = 2;

        // Add a node
        m_space->AddNode(new TestNode(resMan->GetTexture("test_spritesheet")));

        // Create scene
        m_scene = new TestScene(this);

        // Set scene
        SetScene(m_scene);
    }
};

NGINE_GAME_ENTRY {
    // Load icon (must remain in scope until NGINE_RUN_GAME happens)
    Graphics::Image icon(Path::GetExecutableDirectory() / "content" / "test_icon.png");

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
    gameConfig.MaintainResolution = true;

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