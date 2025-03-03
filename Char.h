#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include <SDL_image.h>
#include <SDL.h>

#include "defs.h"

struct Player
{
    SDL_Rect src, dest;

    SDL_RendererFlip flip;

    int dx= 0;
    int dy = 0;

    int HighJump = Hjump;

    bool jump = false;
    bool fall = false;

    Player()
    {
        dest.h = 64;
        dest.w = 64;
        dest.x = SCREEN_WIDTH / 2;
        dest.y = SCREEN_HEIGHT / 2;
    }

    void TurnLeft()
    {
        dx = -speed;
        flip = SDL_FLIP_HORIZONTAL;
    }

    void TurnRight()
    {
        dx = speed;
        flip = SDL_FLIP_NONE;
    }

    void Jump()
    {
        if (!fall)
        {
            dy = -gravity;
            jump = true;
        }
    }

    void Fall()
    {
        if (!jump && fall)
        {
            dy = gravity;
        }
    }

    void Move()
    {
        dest.x += dx;
        dest.y += dy;
        dx = 0;
        if (jump)
        {
            HighJump--;
            if (HighJump == 0)
            {
                jump = false;
                fall = true;
            }
        }
        else if (fall)
        {
            HighJump++;
            if (HighJump == Hjump)
            {
                fall = false;
            }
        }
        if (!jump && !fall) dy = 0;
    }
};

#endif // CHAR_H_INCLUDED
