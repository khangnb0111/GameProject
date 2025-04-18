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
LTexture gExitButtonTexture;
LTexture gBackButtonTexture;
LTexture gPauseButtonTexture;
LTexture gContinueButtonTexture;
LTexture gMenu;
LTexture gExplosion[10];

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
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

    gFont = loadFont("data/pixel_font.ttf", 100);

    SDL_Texture* HighScoreText = renderText("High Score: ", gFont, textColor);

    gJump = loadSound("Music\\Jump.wav");

    gPlayer.loadTexture("Image/character.png", renderer);
    dirt.loadTexture("Image/dirt.jpg", renderer);
    gBackground.loadTexture("Image/forest.jpg", renderer);
    gPlayButtonTexture.loadTexture("Image/play_button.png", renderer);
    gExitButtonTexture.loadTexture("Image/exit_button.png", renderer);
    gBackButtonTexture.loadTexture("Image/back_button.png", renderer);
    gPauseButtonTexture.loadTexture("Image/pause_button.png", renderer);
    gContinueButtonTexture.loadTexture("Image/continue_button.png", renderer);
    gMenu.loadTexture("Image/dogge.jpg", renderer);

    PlayButton.GetButton(gPlayButtonTexture);
    ExitButton.GetButton(gExitButtonTexture);
    BackButton.GetButton(gBackButtonTexture);
    PauseButton.GetButton(gPauseButtonTexture);
    ContinueButton.GetButton(gContinueButtonTexture);

    background.setTexture(gBackground);

    while (running())
    {
        if (!Menu)
        {
            bool keyDown = false;
            if (currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround)
            {
                player.Jump();
                play(gJump);
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

            SDL_RenderClear(renderer);

            renderBackground(background, gBackground);
            renderChar(gPlayer);
            renderMap(Stage, dirt);

            present();

            gameOver(player, Stage, gPlayer);

            SDL_Delay(30);
        }
        else
        {
            HandlePlayButton(PlayButton, gPlayButtonTexture);
            if (HandleExitButton(ExitButton, gExitButtonTexture)) break;

            SDL_RenderCopy(renderer, gMenu.texture, NULL, NULL);
            renderTexture(HighScoreText, HIGH_SCORE_POSX, HIGH_SCORE_POSY, HIGH_SCORE_HEIGHT, HIGH_SCORE_WIDTH);
            renderButton(gPlayButtonTexture);
            renderButton(gExitButtonTexture);
            present();

            SDL_Delay(30);
        }
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);

    SDL_DestroyTexture( HighScoreText );
    TTF_CloseFont( gFont );
    HighScoreText = NULL;

    quit();

    return 0;
}
