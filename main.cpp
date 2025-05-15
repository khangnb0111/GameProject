#include <iostream>

#include "defs.h"
#include "GRAPHIC.h"
#include "Char.h"
#include "Map.h"
#include "Ltexture.h"
#include "BUTTON.h"
#include "Time.h"

using namespace std;

LTexture gBrick;
LTexture gBackground;
LTexture gPlayer;
LTexture gPlayButtonTexture[2];
LTexture gExitButtonTexture[2];
LTexture gBackButtonTexture[2];
LTexture gContinueButtonTexture[2];
LTexture gSettingButtonTexture[2];
LTexture gMusicButton;
LTexture gSoundButton;
LTexture gMenu;
LTexture gExplosion[10];
LTexture gFire[6];
LTexture gHighScoreText;
LTexture gHighScore;
LTexture gScore;
LTexture gScoreText;
LTexture gMusicText;
LTexture gSoundText;

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
Button ExitButton(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY);
Button BackButton(BACK_BUTTON_POSX, BACK_BUTTON_POSY);
Button ContinueButton(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY);
Button SettingButton(SETTING_BUTTON_POSX, SETTING_BUTTON_POSY);
Button MusicButton(MUSIC_BUTTON_POSX, MUSIC_BUTTON_POSY);
Button SoundButton(SOUND_BUTTON_POSX, SOUND_BUTTON_POSY);

Player player;

MAP Stage;

Background background;

int main(int argc, char* argv[])
{

    initSDL();
    gFont = loadFont("data/pixel_font.ttf", 100);
    gJump = loadSound("Music/Jump.wav");
    gGameOver = loadSound("Music/GameOver.mp3");
    gMusic = loadMusic("Music/MARIO.mp3");

    gPlayer.loadTexture("Image/mario.png", renderer);
    gBrick.loadTexture("Image/brick.png", renderer);
    gBackground.loadTexture("Image/black.jpg", renderer);
    gMenu.loadTexture("Image/dogge.jpg", renderer);

    for (int i = 1; i <= 10; i++)
    {
        std::string path = "Image/Circle_explosion/Circle_explosion" + std::to_string(i) + ".png";
        gExplosion[i].loadTexture(path, renderer);
    }

    for (int i = 1; i <= 6; i++)
    {
        std::string path = "Image/Fire/Fire" + std::to_string(i) + ".png";
        gFire[i].loadTexture(path, renderer);
    }

    gHighScoreText.loadText("High Score: ", gFont, textColor[0], renderer);
    gScoreText.loadText("Score: ", gFont, textColor[0], renderer);
    gMusicText.loadText("MUSIC", gFont, textColor[0], renderer);
    gSoundText.loadText("SOUND", gFont, textColor[0], renderer);

    for (int i = 0; i < 2; i++)
    {
        gContinueButtonTexture[i].loadText("Continue", gFont, textColor[i], renderer);
        gPlayButtonTexture[i].loadText(" Play ", gFont, textColor[i], renderer);
        gExitButtonTexture[i].loadText(" Exit ", gFont, textColor[i], renderer);
        gBackButtonTexture[i].loadText(" Back ", gFont, textColor[i], renderer);
        gSettingButtonTexture[i].loadText("Setting", gFont, textColor[i], renderer);
    }



    for (int i = 0; i < 2; i++)
    {
        PlayButton.GetButton(gPlayButtonTexture[i], 2);
        ExitButton.GetButton(gExitButtonTexture[i], 2);
        BackButton.GetButton(gBackButtonTexture[i], 2);
        ContinueButton.GetButton(gContinueButtonTexture[i], 2);
        SettingButton.GetButton(gSettingButtonTexture[i], 2);
    }

    background.setTexture(gBackground);



    while (running())
    {
        fps_time.Start();
        if (scroll < 15)
        {
            scroll += 0.005;
            speed += 0.005;
        }
        if (!Menu)
        {
            playMusic(gMusic);
            if (!fps_time.IsPause)
            {
                bool keyDown = false;

                if (currentKeyStates[SDL_SCANCODE_SPACE])
                {
                    fps_time.Pause();
                    keyDown = true;
                }

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
                gScore.loadText(to_string(Score), gFont, textColor[0], renderer);

                renderBackground(background, gBackground);
                renderChar(gPlayer);
                renderMap(Stage, gBrick, gFire[Score % 6]);
                renderExplosion(gExplosion[Score % 10]);
                renderTexture(gScoreText.texture, SCORE_TEXT_POSX, SCORE_TEXT_POSY, SCORE_TEXT_HEIGHT, SCORE_TEXT_WIDTH);
                renderTexture(gScore.texture, SCORE_POSX, SCORE_POSY, SCORE_HEIGHT, SCORE_WIDTH);

                if (gPlayer.x <= ESize * 2 || gPlayer.y >= SCREEN_HEIGHT - ESize || player.die)
                {
                    gameOver();
                    play(gGameOver);
                }

            }

            else
            {
                HandleContinueButton(ContinueButton, gContinueButtonTexture[ContinueButton.currentFrame]);
                HandleBackButton(BackButton, gBackButtonTexture[BackButton.currentFrame]);

                renderButton(gContinueButtonTexture[ContinueButton.currentFrame]);
                renderButton(gBackButtonTexture[BackButton.currentFrame]);
            }
        }
        else
        {
            if (Setting)
            {
                HandleBackButton(BackButton, gBackButtonTexture[BackButton.currentFrame]);
                HandleMusicButton(MusicButton, gMusicButton);
                HandleSoundButton(SoundButton, gSoundButton);

                renderTexture(gMenu.texture, 0, 0);
                renderButton(gBackButtonTexture[BackButton.currentFrame]);
                renderTexture(gMusicText.texture, MUSIC_TEXT_POSX, MUSIC_TEXT_POSY, MUSIC_TEXT_HEIGHT, MUSIC_TEXT_WIDTH);
                renderTexture(gSoundText.texture, SOUND_TEXT_POSX, SOUND_TEXT_POSY, SOUND_TEXT_HEIGHT, SOUND_TEXT_WIDTH);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = {MUSIC_BUTTON_POSX, MUSIC_BUTTON_POSY, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                DrawThickRect(renderer, rect, 5, MusicButton.currentFrame);
                rect = {SOUND_BUTTON_POSX, SOUND_BUTTON_POSY, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT};
                DrawThickRect(renderer, rect, 5, SoundButton.currentFrame);
            }

            else
            {
                player.dy = 0;
                Mix_HaltMusic();
                player.reset(gPlayer);
                Stage.reset();
                scroll = 4;
                speed = 6;
                UpdateHighScore(Score);
                Score = 0;
                GetHighScore(HighScore);
                gHighScore.loadText(HighScore, gFont, textColor[0], renderer);

                HandlePlayButton(PlayButton, gPlayButtonTexture[PlayButton.currentFrame]);
                HandleSettingButton(SettingButton, gSettingButtonTexture[SettingButton.currentFrame]);
                if (HandleExitButton(ExitButton, gExitButtonTexture[ExitButton.currentFrame])) break;

                renderTexture(gMenu.texture, 0, 0);
                renderTexture(gHighScoreText.texture, HIGH_SCORE_TEXT_POSX, HIGH_SCORE_TEXT_POSY, HIGH_SCORE_TEXT_HEIGHT, HIGH_SCORE_TEXT_WIDTH);
                renderTexture(gHighScore.texture, HIGH_SCORE_POSX, HIGH_SCORE_POSY, HIGH_SCORE_TEXT_HEIGHT, HIGH_SCORE_WIDTH);
                renderButton(gPlayButtonTexture[PlayButton.currentFrame]);
                renderButton(gExitButtonTexture[ExitButton.currentFrame]);
                renderButton(gSettingButtonTexture[SettingButton.currentFrame]);
            }


        }

        present();

        int real_imp_time = fps_time.GetTick();
        int time_one_frame = 1000 / FPS;

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            SDL_Delay(delay_time);
        }
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );
    if (gJump != nullptr) Mix_FreeChunk( gJump);
    if (gGameOver != nullptr) Mix_FreeChunk(gGameOver);

    TTF_CloseFont( gFont );

    quit();

    return 0;
}
