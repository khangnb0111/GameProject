#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include "defs.h"
#include "Map.h"

struct Player
{
    SDL_Texture *texture;

    SDL_Rect src, dest;

    SDL_RendererFlip flip;

    float dx= 0;
    float dy = 0;

    bool isOnGround = false;

    Player()
    {
        dest.h = ESize;
        dest.w = ESize;
        dest.x = SCREEN_WIDTH / 2;
        dest.y = SCREEN_HEIGHT / 2;
    }

    void reset()
    {
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
        dy = -MaxJump;
    }

    bool InStage()
    {
        if (dest.y <= 0) return false;
        if (dest.x <= 0) return false;
        if (dest.y + ESize > SCREEN_HEIGHT) return false;
        if (dest.x + ESize > SCREEN_WIDTH) return false;
        return true;
    }

    void Interact(MAP &Stage)
    {
        int mx, my, adj;
        bool hit;

        if (dx != 0)
        {
            dest.x += dx;

            mx = (dx > 0 ? (dest.x + dest.w + (Stage.scrollingOffset % ESize)) : dest.x + (Stage.scrollingOffset % ESize));
            mx /= ESize;

            my = dest.y / ESize;

            hit = false;

            if (!InStage() || Stage.Map[my][mx] != 0)
            {
                hit = true;
            }

            my = (dest.y + dest.h - 1) / ESize;

            if (!InStage() || Stage.Map[my][mx] != 0)
            {
                hit= true;
            }

            if (hit)
            {
                adj = (dx > 0 ? -dest.w - (Stage.scrollingOffset % ESize) : dest.w - (Stage.scrollingOffset % ESize));

                dest.x = (mx * ESize) + adj;

                dx = 0;
            }
        }

        if (dy != 0)
        {
            dest.y += dy;

            my = (dy > 0 ? (dest.y + dest.h) : dest.y);
            my /= ESize;

            mx = (dest.x + (Stage.scrollingOffset % ESize)) / ESize;

            hit = false;

            if (!InStage() || Stage.Map[my][mx] != 0)
            {
                hit = true;
            }

            mx = (dest.x + dest.w + (Stage.scrollingOffset % ESize) - 1) / ESize;

            if (!InStage() || Stage.Map[my][mx] != 0)
            {
                hit= true;
            }

            if (hit)
            {
                adj = (dy > 0 ? -dest.h : dest.h);

                dest.y = (my * ESize) + adj;

                isOnGround = (dy > 0);

                dy = 0;
            }
        }
    }

    void Move(MAP &Stage)
    {
        dy += gravity;

        isOnGround = false;

        Interact(Stage);

        dx = 0;
    }
};

#endif // CHAR_H_INCLUDED
