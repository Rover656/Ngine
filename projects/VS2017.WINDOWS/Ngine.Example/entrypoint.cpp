#include "stdafx.h"

#include "ngine.h"

#include "Core/Game.h"
#include "Input/Mouse.h"

class TestGame : public Ngine::Core::Game
{
    Ngine::Types::TFont def;
    Ngine::Types::TTexture2D tex;
public:
	TestGame(int width_, int height_, int DrawFPS_, int UpdateFPS_, std::string title_) : Game(width_, height_, DrawFPS_, UpdateFPS_, title_, Ngine::Core::MSAA_4X) {
        // Only works on windows for obvious reasons, this is just for me to test while adding the abstraction layer
        def = Ngine::Types::TFont::LoadFont("C:\\windows\\Fonts\\Arial.ttf");
        //def = Ngine::Types::TFont::GetDefaultFont();

        tex = Ngine::Types::TTexture2D::LoadTexture("A:\\Pictures\\Video Projects\\Rev3_399.png");

        int updateEvent = OnDraw.Bind(this, &TestGame::Draw);
        OnDraw.UnBind(updateEvent);
        updateEvent = OnDraw.Bind(this, &TestGame::Draw);
	}
    
    void Draw(Ngine::Core::EventArgs e_) {
        Ngine::Graphics::Drawing::DrawTriangle({ 250, 0 }, { 100, 200 }, {400, 200}, Ngine::Types::TColor::Yellow);

        Ngine::Graphics::Drawing::DrawText(def, "Hello World", { 10, 10 }, 48, 2, Ngine::Types::TColor::White);

        Ngine::Graphics::Drawing::DrawFPS({ 10, 100 });

        auto pos = Ngine::Input::Mouse::GetMousePosition();

        auto color = Ngine::Input::Mouse::IsButtonDown(Ngine::Input::LEFT);

        Ngine::Graphics::Drawing::DrawTexture(tex, pos, 300, 200, color ? Ngine::Types::TColor::Orange : Ngine::Types::TColor::White);
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	auto game = new TestGame(800, 600, 120, 60, "Hi");
	game->Run();
	delete game;
	return 0;
}