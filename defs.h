#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#define WINDOW_TITLE  "DOGGE"
#define SMALL_BUTTON 1
#define COMMON_BUTTON 2

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

const int ESize = 64;

const int FPS = 30;

const int MAP_HEIGHT = 12;
const int MAP_WIDTH = 20;
float scroll = 4;

float speed = 6;
const float gravity = 1.5;
const int MaxJump = 25;
const int PLAYERFRAMES = 3;

const int COMMON_BUTTON_WIDTH = 150;
const int COMMON_BUTTON_HEIGHT = 98;
const int SMALL_BUTTON_WIDTH = 22;
const int SMALL_BUTTON_HEIGHT = 34;

const int PLAY_BUTON_POSX = 350;
const int PLAY_BUTTON_POSY= 350;
const int EXIT_BUTTON_POSX = 350;
const int EXIT_BUTTON_POSY = 459;
const int BACK_BUTTON_POSX = 31;
const int BACK_BUTTON_POSY = 29;
const int PAUSE_BUTTON_POSX = 31;
const int PAUSE_BUTTON_POSY = 29;
const int CONTINUE_BUTTON_POSX = 31;
const int CONTINUE_BUTTON_POSY = 29;

const int HIGH_SCORE_TEXT_POSX = 200;
const int HIGH_SCORE_TEXT_POSY = 150;
const int HIGH_SCORE_TEXT_HEIGHT = 180;
const int HIGH_SCORE_TEXT_WIDTH = 250;
const int HIGH_SCORE_POSX = 480;
const int HIGH_SCORE_POSY = 150;
const int HIGH_SCORE_HEIGHT = 180;
const int HIGH_SCORE_WIDTH = 150;

const int SCORE_TEXT_POSX = 100;
const int SCORE_TEXT_POSY = 70;
const int SCORE_TEXT_HEIGHT = 90;
const int SCORE_TEXT_WIDTH = 125;
const int SCORE_POSX = 250;
const int SCORE_POSY = 70;
const int SCORE_HEIGHT = 90;
const int SCORE_WIDTH = 90;

SDL_Rect sPlayer[] = {
        {5, 0, 16, 16},
        {35, 0, 16, 16},
        {65, 0, 16, 16},
        {95, 0, 16, 16},
        {155, 0, 16, 16},
        {184, 14, 16, 16}};

SDL_Rect sButton[] = {
        {0, 0, 150, 98},
        {150, 0, 150, 98}};

const int sExplosion = 10;

#endif // DEFS_H_INCLUDED
