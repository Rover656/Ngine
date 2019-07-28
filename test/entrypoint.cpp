#include <ngine.h>

#include <Audio/AudioManager.h>
#include <Audio/Music.h>
#include <Audio/Sound.h>
#include <Components/BoundingBoxCollisionShapeComponent.h>
#include <Components/CameraComponent.h>
#include <Components/CircleCollisionShapeComponent.h>
#include <Components/PolygonCollisionShapeComponent.h>
#include <Components/SpriteComponent.h>
#include <BaseEntity.h>
#include <Component.h>
#include <Game.h>
#include <Resources.h>
#include <Graphics/Sprite.h>
#include <Graphics/TilesetCanvas.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Vector2.h>
#include <Physics/BoundingBox.h>
#include <Physics/Circle.h>
#include <Physics/Polygon.h>
#include <UI/Controls/BasicButton.h>
#include <UI/Controls/HorizontalPanel.h>
#include <UI/Controls/Image.h>
#include <UI/Controls/Label.h>
#include <UI/Controls/VerticalPanel.h>
#include <UI/UIControl.h>
#include <UI/UIPanel.h>
#include <UI/UIWidget.h>
#include <Window.h>

using namespace NGINE_NS;
using namespace NGINE_NS::Audio;
using namespace NGINE_NS::Components;
using namespace NGINE_NS::Graphics;
using namespace NGINE_NS::Input;
using namespace NGINE_NS::Physics;
using namespace NGINE_NS::UI;
using namespace NGINE_NS::UI::Controls;

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 1;//5;

    KeyboardMovementComponent2D(BaseEntity *parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update(EventArgs &e) override {
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
    PlayerEntity(Scene *parentScene_, TVector2 position_)
        : BaseEntity(parentScene_, position_) {
        SetOrigin({16, 16});

        AddComponent("Sprite", new SpriteComponent(this, TSprite(Resources::GetTexture("test_spritesheet"), 16, 16, 32, 32, 30, 0)));
        AddComponent("Movement", new KeyboardMovementComponent2D(this));
        AddComponent("Rectangle", new PolygonCollisionShapeComponent(this, TRectangle(0, 0, 32, 32).ToPolygon()))->
            EnableDebugDraw();

        auto cam = AddComponent("Camera", new CameraComponent(this, 1, {1280/2.0f, 768/2.0f}));

        cam->Activate();
    }
};

class OtherComponent : public Component {
public:
    OtherComponent(BaseEntity *parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update(EventArgs &e) override {
        //tst.Update();

        if (Keyboard::IsKeyPressed(KEY_SPACE)) {
            AudioManager::Play(Resources::GetSound("test_sound"));
        }
    }
};

class OtherEntity : public BaseEntity {
public:
    OtherEntity(Scene *parentScene_) : BaseEntity(parentScene_, TVector2::Zero, 0, true) {
        AddComponent("OtherComponent", new OtherComponent(this));

        AddComponent("TestBoundingBox", new BoundingBoxCollisionShapeComponent(this, {0, 0, 100, 100}))->EnableDebugDraw();
        //AddComponent("TestCircle", new CircleCollisionShapeComponent(this, 50))->EnableDebugDraw();

        //std::vector<TVector2> vertices = { {0, 0}, {100, 0}, {100, 100}, {0, 100} };

        TRectangle rect = {0, 0, 32, 32};

        //AddComponent("TestPolygon", new PolygonCollisionShapeComponent(this, rect.ToPolygon()))->EnableDebugDraw();
    }

    bool CheckForCulling(TRectangle cullArea_) override {
        auto a = cullArea_.ToPolygon();
        auto b = TRectangle(GetPosition(), { 100, 100 }).ToPolygon();
        return a.CheckCollision(&b);
    }
};

class TestWidget : public UIWidget {
public:
    TestWidget(TVector2 pos_) : UIWidget(pos_) {
        auto panelStyle = TUIStyle();
        panelStyle.BackColor = TColor::White;
        panelStyle.BorderColor = TColor::Gray;
        panelStyle.BorderThickness = 2;
        panelStyle.SetPadding(5, 0, 5, 0);
        SetPanel(new VerticalPanel(250.0f, 500.0f));
        //GetPanel<VerticalPanel>()->SetWidth(250.0f);
        //GetPanel<VerticalPanel>()->SetHeight(500.0f);
        GetPanel<VerticalPanel>()->HorizontalAlignment = ALIGN_CENTER;
        //SetPanel(new HorizontalPanel(550.0f, 250.0f));
        //GetPanel<HorizontalPanel>()->EVerticalAlignment = ALIGN_MIDDLE;
        GetPanel()->SetStyle(panelStyle);

        auto style = TUIStyle();
        style.BorderThickness = 2;
        style.BorderColor = TColor::Blue;
        style.SetPadding(5);
        style.BackColor = TColor::Green;
        style.Margin[2] = 5; // 5 bottom margin
        style.Margin[3] = 5; // 5 left margin

        auto l = new Label("Hello world", TFont::GetDefaultFont());
        style.ForeColor = Graphics::TColor::Orange;
        l->SetFontSize(32);
        l->SetStyle(style);
        GetPanel()->AddChild("testLabel", l);

        l = new Label("Hello world", TFont::GetDefaultFont());
        style.ForeColor = Graphics::TColor::Blue;
        l->SetFontSize(62);
        l->SetStyle(style);
        //l->SetConstrainToPanel(true);
        GetPanel()->AddChild("testLabel1", l);

        auto bStyle = TUIStyle();
        bStyle.BackColor = TColor::Blue;

        auto btn = new BasicButton("Test", TFont::GetDefaultFont(), 32, 200, 100);
        btn->OnClick.Bind(&TestWidget::Test);
        btn->SetStyle(bStyle);

        GetPanel()->AddChild("testButton", (UIControl *)btn);

        auto imgStyle = TUIStyle();
        imgStyle.Margin[0] = 5;
        auto img = new Image(TSprite(Resources::GetTexture("test_spritesheet"), 16, 16, 64, 64, 30, 0), 64, 64);
        img->SetStyle(imgStyle);
        //GetPanel()->AddChild("testImage", img);
    }

    static void Test(UIControlEventArgs &e) {
        ConsoleMessage("Button was clicked", "NOTICE", "TestGame Entrypoint");
    }
};

// THIS IS DANGEROUS, DO NOT DO THIS IN PRODUCTION
#include <Graphics/OpenGL/OpenGL.h>

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
    TCamera cam;

    TestWidget widg;

    TilesetCanvas *testTiles;

    TestScene(Game* game) : Scene(game), widg(TVector2(120, 120)) {

        AddEntity("OtherEntity", new OtherEntity(this)); // ->SetRotation(DegToRad(58));

        AddEntity("Player", new PlayerEntity(this, {100, 100}));

        OnLoad.Bind(this, &TestScene::OnLoaded);

        OnDraw.Bind(this, &TestScene::Draw);

        OnUpdate.Bind(this, &TestScene::Update);

        OnDrawCamera.Bind(this, &TestScene::DrawCam);

        SetCullArea(1280 - 50, 768 - 50, true);

        std::vector<int> tileData;
        for (auto i = 0; i < 50; i++) {
            tileData.push_back(1);
        }
        for (auto i = 50; i < 100; i++) {
            tileData.push_back(2);
        }

        testTiles = new TilesetCanvas(TTileset(Resources::GetTexture("test_tiles"), 32, 32), 10, 10, tileData);
        testTiles->SetTileAt({5, 5}, 13);
    }

    ~TestScene() {
        delete testTiles;
    }

    void OnLoaded(SceneLoadEventArgs &e) {
        // AudioManager::Play(Resources::GetMusic("test_music"));

        AudioManager::SetMasterVolume(0.5);
    }

    void Draw(EventArgs &e) {
        // for (auto i = 0; i < 800; i++) {
        //     Renderer::DrawCircle({ 10.0f, 10.0f }, 5, TColor::Orange);
        // }

        //Renderer::DrawFPS({500, 300 });

        //widg.Draw();

//        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);
//        for (auto i = 0; i < 4; i++) {
//            OpenGL::GL::Color({colors[4*i], colors[4*i + 1], colors[4*i + 2], colors[4*i + 3]});
//            OpenGL::GL::Vertex({vertices[3*i], vertices[3*i + 1], vertices[3*i + 2]});
//        }
//        OpenGL::GL::End();

        Renderer::DrawLine({0, 0}, {1280, 768}, TColor::Pink);

        testTiles->Draw({100, 100});
    }

    void DrawCam(EventArgs &e) {
        Renderer::DrawRectangleLines(GetCullArea(), TColor::Green, 1);
    }

    void Update(EventArgs &e) {
        widg.Update();
    }
};

class TestGame : public Game {
    TestScene *_Scene;
public:

    TestGame(int width_, int height_, int DrawFPS_, int UpdateFPS_, std::string title_) : Game(
        width_, height_, 1280, 768, DrawFPS_, UpdateFPS_, title_,
        MAINTAIN_DIMENSIONS | RESIZEABLE_WINDOW) {
        //0) {
        OnRun.Bind(this, &TestGame::Init);
    }

    void Init(EventArgs &e) {
        // Load all content
        auto succ = false;
        // TODO: UWP Support this!!!
        //auto exeDir = Resources::GetExecutableDirectory(succ);
        //Resources::LoadDirectory(exeDir + "\\content");

        // Create scene
        _Scene = new TestScene(this);

        // Set scene
        SetScene(_Scene);
    }
};

#if defined(PLATFORM_DESKTOP)
#ifdef _WIN32
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
#else
int main() {
#endif
#elif defined(PLATFORM_UWP)
#include <Platform/UWP/GameApp.h>

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^) {
#endif
    auto game = new TestGame(1920 / 2, 1080 / 2, 60, 60, "Hi");

#if defined(PLATFORM_UWP)
    CoreApplication::Run(ref new UWP::GameApplicationSource(ref new UWP::GameApp()));
#else
    game->Run();
#endif
    delete game;
    return 0;
}