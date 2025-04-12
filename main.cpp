#include <iostream>
#include <SDL_mixer.h>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"
#include "Map.h"
#include "Ltexture.h"
#include "BUTTON.h"

using namespace std;

LTexture dirt;
LTexture gBackground;
LTexture gPlayer;
LTexture gPlayButtonTexture;
LTexture gHelpButtonTexture;
LTexture gExitButtonTexture;
LTexture gBackButtonTexture;
LTexture gPauseButtonTexture;
LTexture gContinueButtonTexture;
LTexture gMenu;

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
Button HelpButton(HELP_BUTTON_POSX, HELP_BUTTON_POSY);
Button ExitButton(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY);
Button BackButton(BACK_BUTTON_POSX, BACK_BUTTON_POSY);
Button PauseButton(PAUSE_BUTTON_POSX, PAUSE_BUTTON_POSY);
Button ContinueButton(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY);

Player player;

MAP Stage;

Background background;

int main(int argc, char* argv[])
{

    initSDL();

    gPlayer.loadTexture("Image/character.png", renderer);
    dirt.loadTexture("Image/dirt.jpg", renderer);
    gBackground.loadTexture("Image/forest.jpg", renderer);
    gPlayButtonTexture.loadTexture("Image/play_button.png", renderer);
    gHelpButtonTexture.loadTexture("Image/help_button.png", renderer);
    gExitButtonTexture.loadTexture("Image/exit_button.png", renderer);
    gBackButtonTexture.loadTexture("Image/back_button.png", renderer);
    gPauseButtonTexture.loadTexture("Image/pause_button.png", renderer);
    gContinueButtonTexture.loadTexture("Image/continue_button.png", renderer);
    gMenu.loadTexture("Image/dogge.jpg", renderer);

    PlayButton.GetButton(gPlayButtonTexture);
    HelpButton.GetButton(gHelpButtonTexture);
    ExitButton.GetButton(gExitButtonTexture);
    BackButton.GetButton(gBackButtonTexture);
    PauseButton.GetButton(gPauseButtonTexture);
    ContinueButton.GetButton(gContinueButtonTexture);

    //Mix_Music *gMusic = loadMusic("Music\\JoJo.mp3");
    //playMusic(gMusic);
    //Mix_Chunk *gJump = loadSound("Music\\Jump.wav");

    background.setTexture(gBackground);

    while (running())
    {
        if (!Menu)
        {
            bool keyDown = false;
            if (currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround)
            {
                player.Jump();
                //play(gJump);
                keyDown = true;
            }
            if (currentKeyStates[SDL_SCANCODE_LEFT])
            {
                player.TurnLeft();
                gPlayer.flip = SDL_FLIP_HORIZONTAL;
                gPlayer.currentFrame = (gPlayer.currentFrame + 1) % 3;
                keyDown = true;
            }
            if (currentKeyStates[SDL_SCANCODE_RIGHT])
            {
                player.TurnRight();
                gPlayer.flip = SDL_FLIP_NONE;
                gPlayer.currentFrame = (gPlayer.currentFrame + 1) % 3;
                keyDown = true;
            }

            if (!keyDown && player.isOnGround)
            {
                gPlayer.currentFrame = 0;
            }

            player.Move(Stage, gPlayer);

            renderGame(gPlayer, Stage, background, dirt, gBackground);

            present();

            gameOver(player, Stage, gPlayer);

            SDL_Delay(30);
        }
        else
        {
            SDL_RenderCopy(renderer, gMenu.texture, NULL, NULL);
            renderButton(gPlayButtonTexture);
            renderButton(gHelpButtonTexture);
            renderButton(gExitButtonTexture);
            present();
        }
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);

    quit();

    return 0;
}
