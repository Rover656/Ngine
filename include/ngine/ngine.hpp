#ifndef NGINE_NGINE_HPP
#define NGINE_NGINE_HPP

#include "ngine/config.hpp"

#if defined(_WIN32)
#include <Windows.h>
#endif

// Game entrypoint
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
#define NGINE_GAME_ENTRY                                                       \
    int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,         \
                         LPSTR pCmdLine, int nCmdShow)
#else
#define NGINE_GAME_ENTRY int main()
#endif
#elif defined(PLATFORM_UWP)
#define NGINE_GAME_ENTRY                                                       \
    [Platform::MTAThread] int main(Platform::Array<Platform::String ^> ^)
#endif

#endif //NGINE_NGINE_HPP
