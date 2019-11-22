#include <Ngine.h>

#include <Audio/AudioDevice.h>
#include <Audio/Wave.h>
#include <Components/CameraComponent.h>
#include <Components/SpriteComponent.h>
#include <Filesystem/Resources.h>
#include <Graphics/OpenGL/OpenGL.h> // For cheeky hookin

#include <Graphics/Rewrite/Renderer.h>

#include <Graphics/Renderer.h>
#include <Graphics/TilesetRenderer.h>
#include <Input/Keyboard.h>
#include <Physics/Shapes/CircleShape.h>
#include <Physics/Shapes/PolygonShape.h>
//#include <UI/Controls/BasicButton.h>
//#include <UI/Controls/Image.h>
//#include <UI/Controls/Label.h>
//#include <UI/Controls/VerticalPanel.h>
//#include <UI/UIWidget.h>
#include <BaseEntity.h>
#include <Component.h>
#include <Game.h>
#include <Window.h>

using namespace NGINE_NS;
using namespace NGINE_NS::Audio;
using namespace NGINE_NS::Components;
using namespace NGINE_NS::Filesystem;
using namespace NGINE_NS::Graphics;
using namespace NGINE_NS::Input;
using namespace NGINE_NS::Physics;
using namespace NGINE_NS::Physics::Shapes;
using namespace NGINE_NS::UI;
//using namespace NGINE_NS::UI::Controls;

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 32;

    KeyboardMovementComponent2D(BaseEntity *parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update() override {
        auto par = GetParent<BaseEntity>();
        Vector2 velocity;

        if (par->IsPhysicsEntity()) {
            velocity = par->GetPhysicsWorld()->GetGravityPixels();
        }

        if (Keyboard::IsKeyDown(KEY_W)) {
            velocity.Y -= MoveSpeed;
        }

        if (Keyboard::IsKeyDown(KEY_S)) {
            velocity.Y += MoveSpeed;
        }

        if (Keyboard::IsKeyDown(KEY_A)) {
            velocity.X -= MoveSpeed;
        }

        if (Keyboard::IsKeyDown(KEY_D)) {
            velocity.X += MoveSpeed;
        }

        if (par->IsPhysicsEntity()) {
            par->GetPhysicsBody()->SetLinearVelocityPixels(velocity);
        } else {
            par->MoveBy(velocity);
        }

        // Test

        if (Keyboard::IsKeyDown(KEY_LEFT)) {
            par->SetRotation(par->GetRotation() - 5);
        }

        if (Keyboard::IsKeyDown(KEY_RIGHT)) {
            par->SetRotation(par->GetRotation() + 5);
        }

        if (Keyboard::IsKeyDown(KEY_ZERO)) {
            par->SetRotation(0);
        }
    }
};

class PlayerEntity : public BaseEntity {
public:
    PlayerEntity(Scene *parentScene_, Vector2 position_)
            : BaseEntity(parentScene_, position_) {
        PhysicsBody::BodyInfo bi;
        bi.Type = PhysicsBody::BODY_DYNAMIC;
        bi.Position = position_;
        bi.FixedRotation = true;
        auto body = GetParentScene()->GetPhysicsWorld()->CreateBody(bi);
        auto shape = CircleShape(1);
        body->CreateFixture(&shape, 1);
        //auto shapeb = PolygonShape::CreateAsBox(16, 16);
        //body->CreateFixture(&shapeb, 1);
        body->SetAngularVelocity(5);
        SetPhysicsBody(body);

        auto s = AddComponent("Sprite", new SpriteComponent(this, Sprite(Resources::GetTexture("test_spritesheet"), 16, 16, 32, 32, 30, 0)));
        s->SetOrigin({16, 16});

        AddComponent("Movement", new KeyboardMovementComponent2D(this));
        AddComponent("Camera", new CameraComponent(this, 1, {1280/2.0f, 768/2.0f}))->Activate();
    }
};

class Floor : public BaseEntity {
public:
    Floor(Scene *parentScene_, Vector2 position_) : BaseEntity(parentScene_, position_) {
        PhysicsBody::BodyInfo bi;
        bi.Type = PhysicsBody::BODY_STATIC;
        bi.Position = position_;
        auto body = GetParentScene()->GetPhysicsWorld()->CreateBody(bi);
        auto box = PolygonShape::CreateAsBox(5, 5);
        body->CreateFixture(&box, 0);
        SetPhysicsBody(body);
    }
};

//class TestWidget : public UIWidget {
//public:
//    TestWidget(Vector2 pos_) : UIWidget(pos_) {
//        auto fnt = Font::GetDefaultFont();
//        auto panelStyle = TUIStyle();
//        panelStyle.BackColor = Color::White;
//        panelStyle.BorderColor = Color::Gray;
//        panelStyle.BorderThickness = 2;
//        panelStyle.SetPadding(5, 0, 5, 0);
//        SetPanel(new VerticalPanel(250.0f, 500.0f));
//        GetPanel<VerticalPanel>()->HorizontalAlignment = ALIGN_CENTER;
//        GetPanel()->SetStyle(panelStyle);
//
//        auto style = TUIStyle();
//        style.BorderThickness = 2;
//        style.BorderColor = Color::Blue;
//        style.BackColor = Color::Green;
//        style.Margin[2] = 5; // 5 bottom margin
//        style.Margin[3] = 5; // 5 left margin
//
//        auto l = new Label("Hello World", Font::GetDefaultFont());
//        style.ForeColor = Graphics::Color::Orange;
//        l->SetFontSize(32);
//        l->SetStyle(style);
//        GetPanel()->AddChild("testLabel", l);
//
//        l = new Label("Hello world", Font::GetDefaultFont());
//        style.ForeColor = Graphics::Color::Blue;
//        l->SetFontSize(62);
//        l->SetStyle(style);
//        GetPanel()->AddChild("testLabel1", l);
//
//        auto bStyle = TUIStyle();
//        bStyle.BackColor = Color::Blue;
//
//        auto btn = new BasicButton("Test", Font::GetDefaultFont(), 32, 200, 100);
//        btn->OnClick += new FunctionEventHandler<UIControlEventArgs>(&TestWidget::Test);
//        btn->SetStyle(bStyle);
//        GetPanel()->AddChild("testButton", (UIControl *)btn);
//
////        auto imgStyle = TUIStyle();
////        imgStyle.Margin[0] = 5;
////        auto img = new Image(Sprite(Resources::GetTexture("test_spritesheet"), 16, 16, 64, 64, 30, 0), 64, 64);
////        img->SetStyle(imgStyle);
////        //GetPanel()->AddChild("testImage", img);
//    }
//
//    static void Test(UIControlEventArgs e) {
//        ConsoleMessage("Button was clicked", "NOTICE", "TestGame Entrypoint");
//    }
//};

class TestScene : public Scene {
public:
    Camera cam;

    //TestWidget widg;

    TilesetRenderer *testTiles;

    TestScene(Game* game) : Scene(game, true, {0, 0}, 16) {//, widg(Vector2(120, 120)) {
        AddChild("Player", new PlayerEntity(this, {100, 100}));
        AddChild("Floor", new Floor(this, {100, 500}));

        GetPhysicsWorld()->DebugDraw = true;

        OnLoad += new ClassMethodEventHandler<TestScene, SceneLoadEventArgs>(this, &TestScene::OnLoaded);
        OnDrawCamera += new ClassMethodEventHandler<TestScene>(this, &TestScene::Draw);
        OnUpdate += new ClassMethodEventHandler<TestScene>(this, &TestScene::Update);
        OnDrawCamera += new ClassMethodEventHandler<TestScene>(this, &TestScene::DrawCam);

        SetCullArea(1280 - 50, 768 - 50, true);

        std::vector<int> tileData;
        for (auto i = 0; i < 50; i++) {
            tileData.push_back(1);
        }
        for (auto i = 50; i < 100; i++) {
            tileData.push_back(2);
        }

        testTiles = new TilesetRenderer(Tileset(Resources::GetTexture("test_tiles"), 32, 32), 10, 10, tileData);
        testTiles->SetTileAt({5, 5}, 13);
    }

    ~TestScene() {
        delete testTiles;
    }

    void OnLoaded(SceneLoadEventArgs e) {
        //Resources::GetMusic("test_music")->Play();
        AudioDevice::SetMasterVolume(0.25);
    }

    int rot = 0;

    void Draw() {
        ///Renderer::DrawTexture(Resources::GetTexture("test_spritesheet"), {0, 0}, Color::White);
        testTiles->Draw({0, 0}, GetCullAreaPosition(), GetCullAreaEndPosition(), 2.0f);
        ///Renderer::DrawTexture(Resources::GetTexture("test_spritesheet"), {0, 64}, Color::White);
    }

    void DrawCam() {
        ///Renderer::DrawRectangleLines(GetCullArea(), Color::Green, 1);
    }

    void Update() {
        //widg.Update();

        if (Keyboard::IsKeyPressed(KEY_F11)) {
            Window::Config.Fullscreen = !Window::Config.Fullscreen;
            Window::ApplyConfig();
        }
    }
};

class TestGame : public Game {
    TestScene *_Scene;

    Rewrite::GraphicsDevice *_GraphicsDevice;
    Rewrite::Renderer *_Renderer;

    Rewrite::QuadsObject _Obj;

    Texture2D *_Tex;
public:

    TestGame(const GameConfig &config_) : Game(config_) {
        OnInit += new ClassMethodEventHandler<TestGame>(this, &TestGame::Init);
        OnDraw += new ClassMethodEventHandler<TestGame>(this, &TestGame::Draw);
    }

    void Draw() {
        _Renderer->Clear();

        for (auto i = 0; i < 1000; i++)
            _Renderer->AddRenderObject(&_Obj);

        _Renderer->Render();
    }

    void Init() {
        // Set exit key
        Keyboard::SetExitKey(KEY_ESCAPE);

        // EXPERIMENT: New renderer
        _GraphicsDevice = new Rewrite::GraphicsDevice();
        _Renderer = new Rewrite::Renderer(_GraphicsDevice);

        _Renderer->SetClearColor(Color::Blue);

        _Tex = new Texture2D(_GraphicsDevice, Path("content/test_tiles.png"));

        std::vector<Rewrite::VertexData> vdat;
        vdat.push_back({{-0.5f, -0.5f, 0}, Color::Green, {0, 1}});
        vdat.push_back({{0.5f, -0.5f, 0}, Color::Red, {1, 1}});
        vdat.push_back({{0.5f, 0.5f, 0}, Color::Yellow, {1, 0}});
        vdat.push_back({{-0.5f, 0.5f, 0}, Color::Blue, {0, 0}});

//        vdat.push_back({{-0.5f, -0.5f, 0}, Color::White, {0, 1}});
//        vdat.push_back({{0.5f, -0.5f, 0}, Color::White, {1, 1}});
//        vdat.push_back({{0.5f, 0.5f, 0}, Color::White, {1, 0}});
//        vdat.push_back({{-0.5f, 0.5f, 0}, Color::White, {0, 0}});

        _Obj = Rewrite::QuadsObject(vdat);

        _Obj.SetTexture(_Tex);

        return;

        // Load all content
        Resources::LoadResources();

        // Load arial as default font
        Font::SetDefaultFont(Resources::GetFont("Upheaval"));

        // Create scene
        _Scene = new TestScene(this);

        // Set scene
        SetScene(_Scene);
    }
};

#include <NgineEntry.h> // Entrypoint stuff

NGINE_GAME_ENTRY {
    // Load icon
    auto icon = new Graphics::Image(Path::GetExecutableDirectory() / "content" / "test_icon.png");

    GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    //gameConfig.MaintainResolution = true; // DISABLE FOR EXPERIMENT

    WindowConfig windowConfig;
    windowConfig.Resizable = true;
    windowConfig.MSAA_4X = true;
    windowConfig.VSync = false;
    windowConfig.InitialWidth = 1920/2;
    windowConfig.InitialHeight = 1080/2;
    windowConfig.Title = "Test Game";
    windowConfig.Icon = icon;
    windowConfig.NativeDebugConsole = true;
    Window::SetConfig(windowConfig);

    // Create game
    auto game = TestGame(gameConfig);

    // Run game
    NGINE_RUN_GAME(game);

    // Delete icon
    delete icon;

    return 0;
}