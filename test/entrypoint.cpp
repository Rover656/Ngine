#define INCLUDE_WINDOWS
#include "ngine.h"

#include "Core/Game.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

using namespace NGINE_NS;
using namespace NGINE_NS::Core;
using namespace NGINE_NS::Graphics;
using namespace NGINE_NS::Input;

class TestGame : public Game {
    TFont def;
    TTexture2D tex;
public:
    TestGame(int width_, int height_, int DrawFPS_, int UpdateFPS_, std::string title_) : Game(
        width_, height_, 1920, 1080, DrawFPS_, UpdateFPS_, title_, MSAA_4X | MAINTAIN_DIMENSIONS | RESIZEABLE_WINDOW) {
        // Only works on windows for obvious reasons, this is just for me to test while adding the abstraction layer
        //def = TFont::LoadFont("C:\\windows\\Fonts\\Arial.ttf");
        def = TFont::GetDefaultFont();

        tex = TTexture2D::LoadTexture("A:\\Pictures\\Video Projects\\Rev3_399.png");

        OnDraw.Bind(this, &TestGame::Draw);

        TRectangle rect = { 10, 10, 50, 50 };
        auto test = rect.ToBoundingBox(0);

        TRectangle rectB = { 20, 20, 40, 40 };
        auto testB = rectB.ToBoundingBox(0);

        TCircle c = { {10, 10}, 20 };

        auto collided = test.CheckCollision(&testB);
        auto collB = test.CheckCollision(&c);

        def = TFont::LoadFont("C:\\windows\\Fonts\\Arial.ttf");
    }

    void Draw(EventArgs &e_) {
        Drawing::DrawTriangle({250, 0}, {100, 200}, {400, 200}, TColor::Yellow);

        auto tcol = TColor::White;

        if (Keyboard::IsKeyDown(KEY_SPACE)) {
            tcol = TColor::Red;
        }

        Drawing::DrawText(def, "Hello World", {10, 10}, 48, 2, tcol);

        Drawing::DrawFPS({10, 100});

        auto pos = Mouse::GetMousePosition();

        auto color = Mouse::IsButtonDown(LEFT);

        Drawing::DrawTexture(tex, pos, 300, 200, color ? TColor::Orange : TColor::White);
    }
};

#ifdef _WIN32
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    #else
int main() {
    #endif
    auto game = new TestGame(1280, 768, 120, 60, "Hi");
    game->Run();
    delete game;
    return 0;
}
