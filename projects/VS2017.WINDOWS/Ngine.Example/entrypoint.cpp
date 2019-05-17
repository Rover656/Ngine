#include "stdafx.h"

#include "ngine.h"

#include "Core/Game.h"
#include "Core/Scene2D.h"

class TestGame : public Ngine::Core::Game
{
    Ngine::Types::TFont def;
public:
	TestGame(int width_, int height_, int DrawFPS_, int UpdateFPS_, std::string title_) : Game(width_, height_, DrawFPS_, UpdateFPS_, title_) {
        // Only works on windows for obvious reasons, this is just for me to test while adding the abstraction layer
        def = Ngine::Types::TFont::LoadFont("C:\\windows\\Fonts\\Arial.ttf");
	}

    void Draw() override {
        Ngine::Graphics::Drawing::DrawTriangle({ 250, 0 }, { 100, 200 }, {400, 200}, Ngine::Types::TColor::Yellow);

        Ngine::Graphics::Drawing::DrawText(def, "Hello World", { 10, 10 }, 48, 2, Ngine::Types::TColor::White);

        Ngine::Graphics::Drawing::DrawFPS({ 10, 100 });
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	auto game = new TestGame(800, 600, 120, 60, "Hi");
	game->Run();
	delete game;
	return 0;
}