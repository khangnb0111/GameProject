#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define WINDOW_TITLE  "Hello World!"

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

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

const int ESize = 64;

const int MAP_HEIGHT = 12;
const int MAP_WIDTH = 20;
const int scroll = 4;

const float speed = 6;
const float gravity = 1.5;
const int MaxJump = 25;

#endif // DEFS_H_INCLUDED
