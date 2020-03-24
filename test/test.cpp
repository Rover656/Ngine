#include <ngine/ngine.hpp>

#include <ngine/game.hpp>

NGINE_GAME_ENTRY {
    ngine::WindowConfig conf;
    conf.Title = "Test";
    conf.Width = 1280;
    conf.Height = 768;

    ngine::Game game(conf);
    game.run();

    return 0;
}