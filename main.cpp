#include <iostream>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"
#include "Map.h"

using namespace std;

int main(int argc, char* argv[])
{
    Game game;
    game.init();

    Player player;

    MAP Stage;

    while (game.running())
    {
        game.prepare();

        if (game.currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround) player.Jump();
        if (game.currentKeyStates[SDL_SCANCODE_LEFT]) player.TurnLeft();
        if (game.currentKeyStates[SDL_SCANCODE_RIGHT]) player.TurnRight();

        player.Move(Stage);

        game.render(player, Stage);

        game.present();

        SDL_Delay(10);
    }

    game.quit();

    return 0;
}
