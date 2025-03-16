#include <iostream>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"
#include "Map.h"

using namespace std;

int main(int argc, char* argv[])
{
    Player player;

    MAP Stage;

    Background background;

    Game game;
    game.init(background, player, Stage);

    background.setTexture();

    while (game.running(player))
    {
        if (game.currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround) player.Jump();
        if (game.currentKeyStates[SDL_SCANCODE_LEFT]) player.TurnLeft();
        if (game.currentKeyStates[SDL_SCANCODE_RIGHT]) player.TurnRight();

        player.Move(Stage);

        game.render(player, Stage, background);

        game.present();

        SDL_Delay(20);
    }

    game.quit(background, player, Stage);

    return 0;
}
