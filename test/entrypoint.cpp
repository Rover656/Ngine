#include <Ngine.h>

#include <Audio/AudioDevice.h>
#include <Audio/Wave.h>
#include <Components/BoundingBoxCollisionShapeComponent.h>
#include <Components/CameraComponent.h>
#include <Components/CircleCollisionShapeComponent.h>
#include <Components/PolygonCollisionShapeComponent.h>
#include <Components/SpriteComponent.h>
#include <Filesystem/Resources.h>
#include <Graphics/TilesetRenderer.h>
#include <Input/Gamepad.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <UI/Controls/BasicButton.h>
#include <UI/Controls/HorizontalPanel.h>
#include <UI/Controls/Image.h>
#include <UI/Controls/Label.h>
#include <UI/Controls/VerticalPanel.h>
#include <UI/UIWidget.h>
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
using namespace NGINE_NS::UI;
using namespace NGINE_NS::UI::Controls;

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 1;

    KeyboardMovementComponent2D(BaseEntity *parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update() override {
        auto par = GetParent<BaseEntity>();
        if (Keyboard::IsKeyDown(KEY_W)) {
            par->MoveBy({0, -MoveSpeed});
        }

        if (Keyboard::IsKeyDown(KEY_S)) {
            par->MoveBy({0, MoveSpeed});
        }

        if (Keyboard::IsKeyDown(KEY_A)) {
            par->MoveBy({-MoveSpeed, 0});
        }

        if (Keyboard::IsKeyDown(KEY_D)) {
            par->MoveBy({MoveSpeed, 0});
        }

        // Test

        if (Keyboard::IsKeyDown(KEY_LEFT)) {
            par->SetRotation(par->GetRotation() - DegToRad(5));
        }

        if (Keyboard::IsKeyDown(KEY_RIGHT)) {
            par->SetRotation(par->GetRotation() + DegToRad(5));
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
        SetScale(10);
        SetOrigin({16*GetScale(), 16*GetScale()});

        AddComponent("Sprite", new SpriteComponent(this, Sprite(Resources::GetTexture("test_spritesheet"), 16, 16, 32, 32, 30, 0)));
        AddComponent("Movement", new KeyboardMovementComponent2D(this));
        AddComponent("Rectangle", new PolygonCollisionShapeComponent(this, Rectangle(0, 0, 32, 32).ToPolygon()))->EnableDebugDraw();

        AddComponent("Circle", new CircleCollisionShapeComponent(this, Circle({16*GetScale(), 16*GetScale()}, 16)))->EnableDebugDraw();

        AddComponent("Camera", new CameraComponent(this, 1, {1280/2.0f, 768/2.0f}))->Activate();
    }
};

class OtherComponent : public Component {
public:
    OtherComponent(BaseEntity *parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update() override {
        if (Keyboard::IsKeyPressed(KEY_SPACE)) {
            Resources::GetSound("test_sound")->Play();
        }
    }
};

class OtherEntity : public BaseEntity {
public:
    OtherEntity(Scene *parentScene_) : BaseEntity(parentScene_, Vector2::Zero, 0, true) {
        AddComponent("OtherComponent", new OtherComponent(this));
        AddComponent("TestBoundingBox", new BoundingBoxCollisionShapeComponent(this, {0, 0, 100, 100}))->EnableDebugDraw();
        AddComponent("TestCircle", new CircleCollisionShapeComponent(this, Circle(Vector2::Zero, 50)))->EnableDebugDraw();

        SubscribeToUpdate();
    }

    bool CheckForCulling(Rectangle cullArea_) override {
        auto a = cullArea_.ToPolygon();
        auto b = Rectangle(GetPosition(), {100, 100 }).ToPolygon();
        return !a.CheckCollision(&b);
    }
};

class TestWidget : public UIWidget {
public:
    TestWidget(Vector2 pos_) : UIWidget(pos_) {
        auto fnt = Font::GetDefaultFont();
        auto panelStyle = TUIStyle();
        panelStyle.BackColor = Color::White;
        panelStyle.BorderColor = Color::Gray;
        panelStyle.BorderThickness = 2;
        panelStyle.SetPadding(5, 0, 5, 0);
        SetPanel(new VerticalPanel(250.0f, 500.0f));
        GetPanel<VerticalPanel>()->HorizontalAlignment = ALIGN_CENTER;
        GetPanel()->SetStyle(panelStyle);

        auto style = TUIStyle();
        style.BorderThickness = 2;
        style.BorderColor = Color::Blue;
        style.BackColor = Color::Green;
        style.Margin[2] = 5; // 5 bottom margin
        style.Margin[3] = 5; // 5 left margin

        auto l = new Label("Hello World", Font::GetDefaultFont());
        style.ForeColor = Graphics::Color::Orange;
        l->SetFontSize(32);
        l->SetStyle(style);
        GetPanel()->AddChild("testLabel", l);

        l = new Label("Hello world", Font::GetDefaultFont());
        style.ForeColor = Graphics::Color::Blue;
        l->SetFontSize(62);
        l->SetStyle(style);
        GetPanel()->AddChild("testLabel1", l);

        auto bStyle = TUIStyle();
        bStyle.BackColor = Color::Blue;

        auto btn = new BasicButton("Test", Font::GetDefaultFont(), 32, 200, 100);
        btn->OnClick += new FunctionEventHandler<UIControlEventArgs>(&TestWidget::Test);
        btn->SetStyle(bStyle);
        GetPanel()->AddChild("testButton", (UIControl *)btn);

//        auto imgStyle = TUIStyle();
//        imgStyle.Margin[0] = 5;
//        auto img = new Image(Sprite(Resources::GetTexture("test_spritesheet"), 16, 16, 64, 64, 30, 0), 64, 64);
//        img->SetStyle(imgStyle);
//        //GetPanel()->AddChild("testImage", img);
    }

    static void Test(UIControlEventArgs e) {
        ConsoleMessage("Button was clicked", "NOTICE", "TestGame Entrypoint");
    }
};

// Define rectangle
float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
};
float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f
};

class TestScene : public Scene {
public:
    Camera cam;

    TestWidget widg;

    TilesetRenderer *testTiles;

    TestScene(Game* game) : Scene(game), widg(Vector2(120, 120)) {
        AddEntity("OtherEntity", new OtherEntity(this)); // ->SetRotation(DegToRad(58));

        AddEntity("Player", new PlayerEntity(this, {100, 100}));

        OnLoad += new ClassMethodEventHandler<TestScene, SceneLoadEventArgs>(this, &TestScene::OnLoaded);

        //OnDraw.Bind(this, &TestScene::Draw);

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
        //auto p = Mouse::GetMousePosition();

        //widg.Draw();

        //rot += Mouse::GetMouseWheelYDelta();
        //rot += (int)Gamepad::GetAxisValue(GAMEPAD_1, GAMEPAD_AXIS_LEFT_X);

        //Renderer::DrawTexture(Resources::GetTexture("test_tiles"), Mouse::GetMousePosition(), 100, 100, Color::White, {} , DegToRad(rot));

        //if (Keyboard::IsKeyDown(KEY_SPACE) || Gamepad::IsButtonDown(GAMEPAD_1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
            testTiles->Draw({0, 0}, GetCullAreaPosition(), GetCullAreaEndPosition(), 2.0f);
            //testTiles->Draw({0, 0}, 2.0f, 3.14f, {10, 10});
        //}

        //Renderer::DrawCircleLines({100, 100}, 30, Color::Red);
        //Renderer::DrawCircleSectorLines({100, 100}, 10, DegToRad(90), DegToRad(180), 2, Color::Green);

        //Renderer::DrawTexture(Font::GetDefaultFont()->GetTexture(), {100, 100}, Color::White);
    }

    void DrawCam() {
        Renderer::DrawRectangleLines(GetCullArea(), Color::Green, 1);
    }

    void Update() {
        widg.Update();

        if (Keyboard::IsKeyPressed(KEY_F11)) {
            Window::Config.Fullscreen = !Window::Config.Fullscreen;
            Window::ApplyConfig();
        }
    }
};

void TestFunc(std::string module) {
    ConsoleMessage("HELLO", "NOTICE", module);
}

class TestGame : public Game {
    TestScene *_Scene;
public:

    TestGame(const GameConfig &config_) : Game(config_) {
        OnRun += new ClassMethodEventHandler<TestGame>(this, &TestGame::Init);
    }

    void TestMethod(std::string module) {
        ConsoleMessage("HELLO FROM METHOD.", "NOTICE", module);
    }

    void Init() {
        // Set exit key
        Keyboard::SetExitKey(KEY_ESCAPE);

        // Load all content
        Resources::LoadResources();

        // Load arial as default font
        Font::SetDefaultFont(Resources::GetFont("Upheaval"));

        // Output readme
        auto f = File(Path(Path::GetExecutableDirectory() / "content" / "readme.txt"));

        if (f.Open(MODE_READ)) {
            auto t = f.ReadString();
            ConsoleMessage("Readme Reads:", "NOTICE", "entrypoint");
            ConsoleMessage(t, "NOTICE", "entrypoint - readme.txt");
            f.Close();
        }

        // Create scene
        _Scene = new TestScene(this);

        // Set scene
        SetScene(_Scene);


        // Test new events
        Event<std::string> testEvent;

        auto test = new FunctionEventHandler<std::string>(TestFunc);
        auto attachment = testEvent += test;
        auto attachmentB = testEvent += new ClassMethodEventHandler<TestGame, std::string>(this, &TestGame::TestMethod);
        testEvent("HELLOWORLD");

        //NEW::ClassMethodEventHandler<TestGame> testMeth(this, &TestGame::TestMethod);
        //testMeth.Invoke();
    }

    void Uninit(EventArgs &e) {

    }
};

#include <NgineEntry.h> // Entrypoint stuff

NGINE_GAME_ENTRY {
    // Load icon
    auto icon = new Graphics::Image(Path::GetExecutableDirectory() / "content" / "test_icon.png");

    GameConfig gameConfig;
    gameConfig.TargetWidth = 1280;
    gameConfig.TargetHeight = 768;
    gameConfig.MaintainResolution = true;

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