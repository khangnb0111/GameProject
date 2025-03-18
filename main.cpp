#include <iostream>
#include <SDL_mixer.h>

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

    Mix_Music *gMusic = game.loadMusic("Music\\JoJo.mp3");
    //game.playMusic(gMusic);
    Mix_Chunk *gJump = game.loadSound("Music\\Jump.wav");

    background.setTexture();

    while (game.running(player))
    {
        if (game.currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround)
        {
            player.Jump();
            game.play(gJump);
        }
        if (game.currentKeyStates[SDL_SCANCODE_LEFT]) player.TurnLeft();
        if (game.currentKeyStates[SDL_SCANCODE_RIGHT]) player.TurnRight();

        player.Move(Stage);

        game.render(player, Stage, background);

        game.present();

        SDL_Delay(20);
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);

    game.quit(background, player, Stage);

    return 0;
}
