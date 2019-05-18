#include "stdafx.h"

#include "ngine.h"

#include "Core/Game.h"
#include "Input/Mouse.h"

class TestGame : public Ngine::Core::Game {
    TFont def;
    TTexture2D tex;
public:
    TestGame(int width_, int height_, int DrawFPS_, int UpdateFPS_, std::string title_) : Game(
        width_, height_, DrawFPS_, UpdateFPS_, title_, MSAA_4X) {
        // Only works on windows for obvious reasons, this is just for me to test while adding the abstraction layer
        def = TFont::LoadFont("C:\\windows\\Fonts\\Arial.ttf");
        //def = TFont::GetDefaultFont();

        tex = TTexture2D::LoadTexture("A:\\Pictures\\Video Projects\\Rev3_399.png");

        OnDraw.Bind(this, &TestGame::Draw);
    }

    void Draw(EventArgs &e_) {
        Ngine::Graphics::Drawing::DrawTriangle({250, 0}, {100, 200}, {400, 200}, TColor::Yellow);

        Ngine::Graphics::Drawing::DrawText(def, "Hello World", {10, 10}, 48, 2, TColor::White);

        Ngine::Graphics::Drawing::DrawFPS({10, 100});

        auto pos = Ngine::Input::Mouse::GetMousePosition();

        auto color = Ngine::Input::Mouse::IsButtonDown(LEFT);

        Ngine::Graphics::Drawing::DrawTexture(tex, pos, 300, 200, color ? TColor::Orange : TColor::White);
    }
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    auto game = new TestGame(800, 600, 120, 60, "Hi");
    game->Run();
    delete game;
    return 0;
}
