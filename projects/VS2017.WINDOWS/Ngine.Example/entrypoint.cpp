#include "stdafx.h"

//Temp while we don't have a drawing class
#define INCLUDE_RAYLIB //To use raylib, we need to reference raylib in our project...
#include "ngine.h"

#include "Core/Game.h"
#include "Core/Scene2D.h"

class TestGame : public Ngine::Core::Game
{
public:
	TestGame(int width_, int height_, int FPS_, std::string title_) : Game(width_, height_, FPS_, title_) {}

    void Draw() override {
		//DrawText("Hello World", 80, 80, 24, BLUE);

        Ngine::Graphics::Drawing::DrawTriangle({ 250, 0 }, { 100, 200 }, {400, 200}, Ngine::Types::Yellow);
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	auto game = new TestGame(800, 600, 60, "Hi");
	game->Run();
	delete game;
	return 0;
}