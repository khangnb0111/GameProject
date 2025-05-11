#include <iostream>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"
#include "Map.h"
#include "Ltexture.h"
#include "BUTTON.h"

using namespace std;

LTexture gBrick;
LTexture gSpike;
LTexture gBackground;
LTexture gPlayer;
LTexture gPlayButtonTexture;
LTexture gExitButtonTexture;
LTexture gBackButtonTexture;
LTexture gPauseButtonTexture;
LTexture gContinueButtonTexture;
LTexture gMenu;
LTexture gExplosion[10];
LTexture gHighScoreText;
LTexture gHighScore;
LTexture gScore;
LTexture gScoreText;

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

    gJump = loadSound("Music/Jump.wav");

    gPlayer.loadTexture("Image/mario.png", renderer);
    gBrick.loadTexture("Image/brick.png", renderer);
    gSpike.loadTexture("Image/spike.png", renderer);
    gBackground.loadTexture("Image/black.jpg", renderer);
    gPlayButtonTexture.loadTexture("Image/play_button.png", renderer);
    gExitButtonTexture.loadTexture("Image/exit_button.png", renderer);
    gBackButtonTexture.loadTexture("Image/back_button.png", renderer);
    gPauseButtonTexture.loadTexture("Image/pause_button.png", renderer);
    gContinueButtonTexture.loadTexture("Image/continue_button.png", renderer);
    gMenu.loadTexture("Image/dogge.jpg", renderer);
    gExplosion[0].loadTexture("Image/Circle_explosion/Circle_explosion10.png", renderer);
    gExplosion[1].loadTexture("Image/Circle_explosion/Circle_explosion1.png", renderer);
    gExplosion[2].loadTexture("Image/Circle_explosion/Circle_explosion2.png", renderer);
    gExplosion[3].loadTexture("Image/Circle_explosion/Circle_explosion3.png", renderer);
    gExplosion[4].loadTexture("Image/Circle_explosion/Circle_explosion4.png", renderer);
    gExplosion[5].loadTexture("Image/Circle_explosion/Circle_explosion5.png", renderer);
    gExplosion[6].loadTexture("Image/Circle_explosion/Circle_explosion6.png", renderer);
    gExplosion[7].loadTexture("Image/Circle_explosion/Circle_explosion7.png", renderer);
    gExplosion[8].loadTexture("Image/Circle_explosion/Circle_explosion8.png", renderer);
    gExplosion[9].loadTexture("Image/Circle_explosion/Circle_explosion9.png", renderer);

    gHighScoreText.loadText("High Score: ", gFont, textColor, renderer);
    gScoreText.loadText("Score: ", gFont, textColor, renderer);

    PlayButton.GetButton(gPlayButtonTexture);
    ExitButton.GetButton(gExitButtonTexture);
    BackButton.GetButton(gBackButtonTexture);
    PauseButton.GetButton(gPauseButtonTexture);
    ContinueButton.GetButton(gContinueButtonTexture);

    background.setTexture(gBackground);

    while (running())
    {
        int gettick = SDL_GetTicks();
        if (scroll < 20)
        {
            scroll += 0.005;
            speed += 0.005;
        }
        if (!Menu)
        {
            bool keyDown = false;

            if (currentKeyStates[SDL_SCANCODE_UP] && player.isOnGround)
            {
                player.Jump();
                play(gJump);
                player.frames = PLAYERFRAMES - 1;
                keyDown = true;
            }

            if (currentKeyStates[SDL_SCANCODE_LEFT])
            {
                player.frames++;
                player.TurnLeft();
                gPlayer.flip = SDL_FLIP_HORIZONTAL;
                if (player.frames == PLAYERFRAMES)
                {
                    gPlayer.currentFrame = (gPlayer.currentFrame + 1) % 3;
                    player.frames = 0;
                }
                keyDown = true;
            }

            if (currentKeyStates[SDL_SCANCODE_RIGHT])
            {
                player.frames++;
                player.TurnRight();
                gPlayer.flip = SDL_FLIP_NONE;
                if (player.frames == PLAYERFRAMES)
                {
                    gPlayer.currentFrame = (gPlayer.currentFrame + 1) % 3;
                    player.frames = 0;
                }
                keyDown = true;
            }

            if (!keyDown && player.isOnGround)
            {
                gPlayer.currentFrame = 0;
                player.frames = PLAYERFRAMES - 1;
            }

            player.Move(Stage, gPlayer);

            SDL_RenderClear(renderer);

            Score++;
            gScore.loadText(to_string(Score), gFont, textColor, renderer);

            renderBackground(background, gBackground);
            renderChar(gPlayer);
            renderMap(Stage, gBrick, gSpike);
            renderExplosion(gExplosion[Score % 10]);
            renderTexture(gScoreText.texture, SCORE_TEXT_POSX, SCORE_TEXT_POSY, SCORE_TEXT_HEIGHT, SCORE_TEXT_WIDTH);
            renderTexture(gScore.texture, SCORE_POSX, SCORE_POSY, SCORE_HEIGHT, SCORE_WIDTH);

            present();

            gameOver(player, Stage, gPlayer);
        }
        else
        {
            player.reset(gPlayer);
            Stage.reset();
            scroll = 4;
            speed = 6;
            UpdateHighScore(Score);
            Score = 0;
            GetHighScore(HighScore);
            gHighScore.loadText(HighScore, gFont, textColor, renderer);

            HandlePlayButton(PlayButton, gPlayButtonTexture);
            if (HandleExitButton(ExitButton, gExitButtonTexture)) break;

            SDL_RenderCopy(renderer, gMenu.texture, NULL, NULL);
            renderTexture(gHighScoreText.texture, HIGH_SCORE_TEXT_POSX, HIGH_SCORE_TEXT_POSY, HIGH_SCORE_TEXT_HEIGHT, HIGH_SCORE_TEXT_WIDTH);
            renderTexture(gHighScore.texture, HIGH_SCORE_POSX, HIGH_SCORE_POSY, HIGH_SCORE_TEXT_HEIGHT, HIGH_SCORE_WIDTH);
            renderButton(gPlayButtonTexture);
            renderButton(gExitButtonTexture);
            present();
        }

        int real_imp_time = SDL_GetTicks() - gettick;
        int time_one_frame = 1000 / FPS;

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            SDL_Delay(delay_time);
        }
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);

    TTF_CloseFont( gFont );

    quit();

    return 0;
}
