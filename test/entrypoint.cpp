#define INCLUDE_WINDOWS
#include "ngine.h"

#include "Components/CameraComponent.h"
#include "Core/Component.h"
#include "Core/Game.h"
#include "Core/BaseEntity.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

using namespace NGINE_NS;
using namespace NGINE_NS::Components;
using namespace NGINE_NS::Core;
using namespace NGINE_NS::Graphics;
using namespace NGINE_NS::Input;

class HelloWorldComponent2D : public Component {
    TFont def;
public:
    HelloWorldComponent2D(BaseEntity* parent_) : Component(parent_) {
        SubscribeToCameraDraw();

        def = TFont::GetDefaultFont();
    }

    void DrawCamera(EventArgs &e) override {
        Drawing::DrawText(def, "I'm in the scene, controlling the camera\nI am also affected by said camera.", GetParent<BaseEntity>()->GetPosition(), 48, 1, TColor::Red);
    }
};

class KeyboardMovementComponent2D : public Component {
public:
    float MoveSpeed = 5;
    KeyboardMovementComponent2D(BaseEntity* parent_) : Component(parent_) {
        SubscribeToUpdate();
    }

    void Update(EventArgs &e) override {
        auto par = GetParent<BaseEntity>();
        if (Keyboard::IsKeyDown(KEY_W)) {
            par->MoveBy({ 0, -MoveSpeed });
        }

        if (Keyboard::IsKeyDown(KEY_S)) {
            par->MoveBy({ 0, MoveSpeed });
        }

        if (Keyboard::IsKeyDown(KEY_A)) {
            par->MoveBy({ -MoveSpeed, 0 });
        }

        if (Keyboard::IsKeyDown(KEY_D)) {
            par->MoveBy({ MoveSpeed, 0 });
        }
    }
};

class TestEntity : public BaseEntity {
public:
    TestEntity(Scene* parentScene_, const TVector2 &pos_) : BaseEntity(parentScene_, pos_) {
        AddComponent("HelloWorld", new HelloWorldComponent2D(this));
        AddComponent("Movement", new KeyboardMovementComponent2D(this));
        auto cam = AddComponent("Camera", new CameraComponent(this, 1, {50, 50}));

        cam->Activate();
    }
};

class OtherComponent : public Component {
    TFont def;
public:
    OtherComponent(BaseEntity* parent_) : Component(parent_) {
        SubscribeToCameraDraw();
        SubscribeToDraw();

        def = TFont::GetDefaultFont();
    }

    void Draw(EventArgs &e) override {
        // This is like UI, it is not affected by the camera
        auto pos = Mouse::GetMousePosition();

        auto color = Mouse::IsButtonDown(LEFT);

        Drawing::DrawCircle(pos, 15, color ? TColor::Orange : TColor::White);

        Drawing::DrawText(def, "I am in the scene, not affected by the camera", { 25, 300 }, 48, 2, TColor::Purple);
    }

    void DrawCamera(EventArgs &e) override {
        // This is all affected by the camera
        Drawing::DrawTriangle({ 250, 0 }, { 100, 200 }, { 400, 200 }, TColor::Yellow);

        auto tcol = TColor::White;

        if (Keyboard::IsKeyDown(KEY_SPACE)) {
            tcol = TColor::Red;
        }

        Drawing::DrawText(def, "I am in the scene, affected by the camera", { 10, 10 }, 48, 2, tcol);

        Drawing::DrawFPS({ 10, 100 });
    }
};

class OtherEntity : public BaseEntity {
public:
    OtherEntity(Scene* parentScene_) : BaseEntity(parentScene_, TVector2::Zero) {
        AddComponent("OtherComponent", new OtherComponent(this));
    }
};

class TestScene : public Scene {
public:
    TCamera cam;
    TestScene() : Scene() {
        AddEntity("TestEntity", new TestEntity(this, {50, 100}));
        AddEntity("OtherEntity", new OtherEntity(this));
    }
};

class TestGame : public Game {
    TestScene* _Scene;
public:
    TFont def;
    TestGame(int width_, int height_, int DrawFPS_, int UpdateFPS_, std::string title_) : Game(
        width_, height_, 1920, 1080, DrawFPS_, UpdateFPS_, title_, MSAA_4X | MAINTAIN_DIMENSIONS | RESIZEABLE_WINDOW | VSYNC) {
        def = TFont::GetDefaultFont();

        //OnDraw.Bind(this, &TestGame::Draw);

        _Scene = new TestScene();

        SetScene(_Scene);
    }

    void Draw(EventArgs &e_) {
        

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
