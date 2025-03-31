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
Ltexture gStartButton;
Ltexture gRestartButton;

int main(int argc, char* argv[])
{
    Player player;

    MAP Stage;

    Background background;

    Game game;
    game.init(gBackground, gPlayer, dirt, gStartButton, gRestartButton);

    Mix_Music *gMusic = game.loadMusic("Music\\JoJo.mp3");
    game.playMusic(gMusic);
    Mix_Chunk *gJump = game.loadSound("Music\\Jump.wav");

    background.setTexture(gBackground);

    while (game.menu())
    {
        game.renderMenu(gStartButton);
        game.present();
    }

    while (game.running())
    {
        if (game.start)
        {
            if (game.currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround)
            {
                player.Jump();
                game.play(gJump);
            }
            if (game.currentKeyStates[SDL_SCANCODE_LEFT])
            {
                player.TurnLeft();
                gPlayer.flip = SDL_FLIP_HORIZONTAL;
            }
            if (game.currentKeyStates[SDL_SCANCODE_RIGHT])
            {
                player.TurnRight();
                gPlayer.flip = SDL_FLIP_NONE;
            }

            player.Move(Stage, gPlayer);

            game.renderGame(gPlayer, Stage, background, dirt, gBackground);

            game.present();

            game.gameOver(player, Stage, gPlayer);

            SDL_Delay(20);
        }

        else
        {
            game.renderTexture(gRestartButton.texture, 576, 352, 64, 128);

            game.present();

            SDL_Delay(20);
        }
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);

    game.quit(gPlayer, gStartButton, dirt, gBackground, gRestartButton);

    return 0;
}
