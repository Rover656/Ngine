#ifndef NGINEENTRY_H
#define NGINEENTRY_H

#include "Ngine.h"

// Game entrypoint
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
#define NGINE_GAME_ENTRY int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
#define NGINE_GAME_ENTRY int main()
#endif
#elif defined(PLATFORM_UWP)
#define NGINE_GAME_ENTRY [Platform::MTAThread] int main(Platform::Array<Platform::String^>^)
#endif

// Game runner
#if defined(PLATFORM_UWP)
#include "Platform/UWP/GameApp.h"
#define NGINE_RUN_GAME(game) UWP::UWPGameBootstrapper::ExecuteGame(&game); CoreApplication::Run(ref new UWP::GameApplicationSource(ref new UWP::GameApp()));
#define NGINE_RUN_GAME_PTR(game) UWP::UWPGameBootstrapper::ExecuteGame(game); CoreApplication::Run(ref new UWP::GameApplicationSource(ref new UWP::GameApp()));
#else
#define NGINE_RUN_GAME(game) game.Run();
#define NGINE_RUN_GAME_PTR(game) game->Run();
#endif

#endif //NGINEENTRY_H
