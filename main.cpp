#include <iostream>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"

using namespace std;

int main(int argc, char* argv[])
{
    Game game;
    game.init();

    Player player;

    while (game.running())
    {
        game.prepare();

        if (game.currentKeyStates[SDL_SCANCODE_UP]) player.Jump();
        if (game.currentKeyStates[SDL_SCANCODE_LEFT]) player.TurnLeft();
        if (game.currentKeyStates[SDL_SCANCODE_RIGHT]) player.TurnRight();
        player.Fall();

        player.Move();

        game.render(player);

        game.present();

        //SDL_Delay(10);
    }

    game.quit();

    return 0;
}
