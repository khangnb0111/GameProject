#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#define WINDOW_TITLE  "Hello World!"
#define SMALL_BUTTON 1
#define COMMON_BUTTON 2

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

const int ESize = 64;

const int MAP_HEIGHT = 12;
const int MAP_WIDTH = 20;
const int scroll = 4;

const float speed = 6;
const float gravity = 1.5;
const int MaxJump = 25;

const int COMMON_BUTTON_WIDTH = 150;
const int COMMON_BUTTON_HEIGHT = 98;
const int SMALL_BUTTON_WIDTH = 22;
const int SMALL_BUTTON_HEIGHT = 34;

const int PLAY_BUTON_POSX = 389;
const int PLAY_BUTTON_POSY= 186;
const int HELP_BUTTON_POSX = 389;
const int HELP_BUTTON_POSY = 293;
const int EXIT_BUTTON_POSX = 389;
const int EXIT_BUTTON_POSY = 402;
const int BACK_BUTTON_POSX = 31;
const int BACK_BUTTON_POSY = 29;
const int PAUSE_BUTTON_POSX = 31;
const int PAUSE_BUTTON_POSY = 29;
const int CONTINUE_BUTTON_POSX = 31;
const int CONTINUE_BUTTON_POSY = 29;

SDL_Rect sPlayer[] = {
        {6, 0, 18, 24},
        {38, 0, 18, 24},
        {70, 0, 18, 24},
        {102, 0, 18, 24},
        {134, 0, 18, 24},
        {166, 0, 18, 24},
        {198, 0, 18, 24},
        {230, 0, 18, 24},
        {262, 0, 18, 24}};

SDL_Rect sButton[] = {
        {0, 0, 150, 98},
        {150, 0, 150, 98}};

#endif // DEFS_H_INCLUDED
