#include <iostream>
#include <SDL_mixer.h>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"
#include "Map.h"
#include "Ltexture.h"

using namespace std;

Ltexture dirt;
Ltexture gBackground;
Ltexture gPlayer;

int main(int argc, char* argv[])
{
    Player player;

    MAP Stage;

    Background background;

    Menu menu;

    Game game;
    game.init(gBackground, player, dirt, menu);

    Mix_Music *gMusic = game.loadMusic("Music\\JoJo.mp3");
    game.playMusic(gMusic);
    Mix_Chunk *gJump = game.loadSound("Music\\Jump.wav");

    background.setTexture(gBackground);

    while (game.running(player))
    {
        if (game.start)
        {
            if (game.currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround)
            {
                player.Jump();
                game.play(gJump);
            }
            if (game.currentKeyStates[SDL_SCANCODE_LEFT]) player.TurnLeft();
            if (game.currentKeyStates[SDL_SCANCODE_RIGHT]) player.TurnRight();

            player.Move(Stage);

            game.render(player, Stage, background, dirt, gBackground);

            game.present();

            game.gameOver(player, Stage);

            SDL_Delay(20);
        }

        else
        {
            game.rendermenu(menu);
            game.present();
        }
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);

    game.quit(player, menu, dirt, gBackground);

    return 0;
}
