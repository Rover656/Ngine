#define INCLUDE_WINDOWS
#include "ngine.h"

#include <Audio/AudioManager.h>
#include <Audio/Music.h>
#include <Audio/Sound.h>
#include <Components/BoundingBoxCollisionShapeComponent.h>
#include <Components/CameraComponent.h>
#include <Components/CircleCollisionShapeComponent.h>
#include <Components/PolygonCollisionShapeComponent.h>
#include <Components/SpriteComponent.h>
#include <Core/BaseEntity.h>
#include <Core/Component.h>
#include <Core/Game.h>
#include <Core/Resources.h>
#include <Graphics/Sprite.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Math/Vector2.h>
#include <Physics/BoundingBox.h>
#include <Physics/Circle.h>
#include <Physics/Polygon.h>
#include <UI/Controls/Label.h>
#include <UI/Controls/VerticalPanel.h>
#include <UI/UIControl.h>
#include <UI/UIPanel.h>
#include <UI/UIWidget.h>

using namespace NGINE_NS;
using namespace NGINE_NS::Audio;
using namespace NGINE_NS::Components;
using namespace NGINE_NS::Core;
using namespace NGINE_NS::Graphics;
using namespace NGINE_NS::Input;
using namespace NGINE_NS::Math;
using namespace NGINE_NS::Physics;
using namespace NGINE_NS::UI;
using namespace NGINE_NS::UI::Controls;

class HelloWorldComponent2D : public Component {
public:
    HelloWorldComponent2D(BaseEntity *parent_) : Component(parent_) {
        SubscribeToCameraDraw();
    }

    void DrawCamera(EventArgs &e) override {
        Drawing::DrawText(TFont::GetDefaultFont(),
                          "I'm in the scene, controlling the camera\nI am also affected by said camera.",
                          GetParent<BaseEntity>()->GetPosition(), 48, 1, TColor::Red);
    }
};

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 5;

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

class TestEntity : public BaseEntity {
public:
    TestEntity(Scene *parentScene_, const TVector2 &pos_) : BaseEntity(parentScene_, pos_) {
        AddComponent("HelloWorld", new HelloWorldComponent2D(this));

        //AddComponent("TestBoundingBox", new BoundingBoxCollisionShapeComponent(this, { 0, 0, 100, 100 }));

        TRectangle bounds = {0, 0, 100, 100};
        std::vector<TVector2> vertices = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};

        //AddComponent("TestPolygon", new PolygonCollisionShapeComponent(this, vertices))->EnableDebugDraw(true);
    }
};

class PlayerEntity : public BaseEntity {
public:
    PlayerEntity(Scene *parentScene_, TVector2 position_)
        : BaseEntity(parentScene_, position_) {
        SetOrigin({32, 32});

        AddComponent("Sprite", new SpriteComponent(this, TSprite(Resources::GetTexture("test_spritesheet"), 16, 16, 64, 64, 30, 0)));
        AddComponent("Movement", new KeyboardMovementComponent2D(this));
        AddComponent("Rectangle", new PolygonCollisionShapeComponent(this, TRectangle(0, 0, 64, 64).ToPolygon()))->
            EnableDebugDraw(true);

        auto cam = AddComponent("Camera", new CameraComponent(this, 1, {1280/2.0f, 768/2.0f}));

        cam->Activate();
    }
};

class OtherComponent : public Component {
    TSprite tst;
public:
    OtherComponent(BaseEntity *parent_) : Component(parent_) {
        SubscribeToCameraDraw();
        SubscribeToDraw();
        SubscribeToUpdate();
    }

    void Draw(EventArgs &e) override {
        // This is like UI, it is not affected by the camera
        auto pos = Mouse::GetMousePosition();

        auto color = Mouse::IsButtonDown(LEFT);

        Drawing::DrawCircle(pos, 15, color ? TColor::Orange : TColor::White);

        //Drawing::DrawText(TFont::GetDefaultFont(), "I am in the scene, not affected by the camera", {25, 300}, 48, 2,
        //                  TColor::Purple);
    }

    void Update(EventArgs &e) override {
        //tst.Update();

        if (Keyboard::IsKeyPressed(KEY_SPACE)) {
            AudioManager::Play(Resources::GetSound("test_sound"));
        }
    }

    void DrawCamera(EventArgs &e) override {
        //tst.Draw({100, 250}, 0);

        // This is all affected by the camera
        Drawing::DrawTriangle({250, 0}, {100, 200}, {400, 200}, TColor::Yellow);

        auto tcol = TColor::White;

        if (Keyboard::IsKeyDown(KEY_SPACE)) {
            tcol = TColor::Red;
        }

        Drawing::DrawText(TFont::GetDefaultFont(), "I am in the scene, affected by the camera", {10, 10}, 48, 2,
                          tcol);

        Drawing::DrawFPS({10, 100});

        for (auto i = 0; i < 800; i++) {
            Drawing::DrawCircle({ 10.0f, 10.0f }, 5, TColor::Orange);
        }
    }
};

class OtherEntity : public BaseEntity {
public:
    OtherEntity(Scene *parentScene_) : BaseEntity(parentScene_, TVector2::Zero, 0, true) {
        AddComponent("OtherComponent", new OtherComponent(this));

        AddComponent("TestBoundingBox", new BoundingBoxCollisionShapeComponent(this, {0, 0, 100, 100}))->
            EnableDebugDraw(true);
        //AddComponent("TestCircle", new CircleCollisionShapeComponent(this, 50))->EnableDebugDraw(true);

        //std::vector<TVector2> vertices = { {0, 0}, {100, 0}, {100, 100}, {0, 100} };

        TRectangle rect = {0, 0, 100, 100};

        //AddComponent("TestPolygon", new PolygonCollisionShapeComponent(this, rect.ToPolygon()))->EnableDebugDraw(true);
    }

    bool CheckForCulling(Math::TRectangle cullArea_) override {
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
        panelStyle.Padding[0] = 5;
        SetPanel(new VerticalPanel(250.0f, 500.0f));
        GetPanel<VerticalPanel>()->HorizontalAlignment = ALIGN_CENTER;
        GetPanel()->SetStyle(panelStyle);

        auto style = TUIStyle();
        style.BorderThickness = 2;
        style.BorderColor = TColor(0, 0, 255, 255 / 4);
        style.Padding[0] = style.Padding[1] = style.Padding[2] = style.Padding[3] = 5;
        style.BackColor = TColor::Green;
        style.Margin[2] = 5; // 5 bottom margin

        auto l = new Label("Hello world", TFont::GetDefaultFont());
        style.ForeColor = Graphics::TColor::Orange;
        l->SetFontSize(32);
        l->SetStyle(style);
        GetPanel()->AddChild("testLabel", l);

        l = new Label("Hello world", TFont::GetDefaultFont());
        style.ForeColor = Graphics::TColor::Blue;
        l->SetFontSize(48);
        l->SetStyle(style);
        GetPanel()->AddChild("testLabel1", l);
    }
};

class TestScene : public Scene {
public:
    TCamera cam;

    TestWidget widg;

    TestScene(Game* game) : Scene(game), widg(TVector2(120, 120)) {
        auto test = AddEntity("TestEntity", new TestEntity(this, {50, 100}));
        test->SetOrigin(TVector2(50, 50));

        AddEntity("OtherEntity", new OtherEntity(this)); // ->SetRotation(DegToRad(58));

        AddEntity("Player", new PlayerEntity(this, {100, 100}));

        OnLoad.Bind(this, &TestScene::OnLoaded);

        OnDraw.Bind(this, &TestScene::Draw);

        OnDrawCamera.Bind(this, &TestScene::DrawCam);

        SetCullArea(1280 - 50, 768 - 50, true);
    }

    void OnLoaded(SceneLoadEventArgs &e) {
        // AudioManager::Play(Resources::GetMusic("test_music"));

        AudioManager::SetMasterVolume(0.5);
    }

    void Draw(EventArgs &e) {
        // for (auto i = 0; i < 800; i++) {
        //     Drawing::DrawCircle({ 10.0f, 10.0f }, 5, TColor::Orange);
        // }

        Drawing::DrawFPS({ 500, 300 });

        widg.Draw();
    }

    void DrawCam(EventArgs &e) {
        Drawing::DrawRectangleLines(GetCullArea(), TColor::Green, 1);
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
        OnRun.Bind(this, &TestGame::Init);
    }

    void Init(EventArgs &e) {
        // Load all content
        auto succ = false;
        auto exeDir = Resources::GetExecutableDirectory(succ);
        Resources::LoadDirectory(exeDir + "\\content");

        // Create scene
        _Scene = new TestScene(this);

        // Set scene
        SetScene(_Scene);
    }
};

#ifdef _WIN32
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    #else
int main() {
    #endif
    auto game = new TestGame(1280, 768, 60, 60, "Hi");
    game->Run();
    delete game;
    return 0;
}
