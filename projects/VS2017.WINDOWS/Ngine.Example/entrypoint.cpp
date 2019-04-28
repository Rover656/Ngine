#include "stdafx.h"

#include "3rd-party/raylib/src/raylib.h"
#include "Core/Game.h"

class TestGame : public Ngine::Core::Game
{
public:
	TestGame(int width_, int height_, int FPS_, std::string title_) : Game(width_, height_, FPS_, title_) {}

    void Draw() override {
		DrawText("Hello World", 80, 80, 24, BLUE);
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	auto game = new TestGame(800, 600, 60, "Hi");
	game->Run();
	delete game;
	return 0;
}