#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include "defs.h"
#include "Ltexture.h"
#include "Map.h"

struct Player
{
    float dx= 0;
    float dy = 0;
    int frames = 0;

    bool isOnGround = false;

    void reset(LTexture &gPlayer)
    {
        gPlayer.x = SCREEN_WIDTH / 2;
        gPlayer.y = SCREEN_HEIGHT / 2;
    }

    void TurnLeft()
    {
        dx = -speed;
    }

    void TurnRight()
    {
        dx = speed;
    }

    void Jump()
    {
        dy = -MaxJump;
    }

    void InScreen(LTexture &gPlayer)
    {
        if (gPlayer.x + ESize > SCREEN_WIDTH)
        {
            gPlayer.x = SCREEN_WIDTH - ESize;
            return;
        }
        return;
    }

    void Interact(MAP &Stage, LTexture &gPlayer)
    {
        int mx, my, adj;
        bool hit;

        if (dx != 0)
        {
            gPlayer.x += dx;

            mx = (dx > 0 ? (gPlayer.x + gPlayer.w + Stage.scrollingOffset) : gPlayer.x + Stage.scrollingOffset);
            mx /= ESize;

            my = gPlayer.y / ESize;

            hit = false;

            if (Stage.Map[my][mx] != 0)
            {
                hit = true;
            }

            my = (gPlayer.y + gPlayer.h - 1) / ESize;

            if (Stage.Map[my][mx] != 0)
            {
                hit= true;
            }

            if (hit)
            {
                adj = (dx > 0 ? -gPlayer.w - Stage.scrollingOffset : gPlayer.w - Stage.scrollingOffset);

                gPlayer.x = (mx * ESize) + adj;

                dx = 0;
            }
        }

        if (dy != 0)
        {
            gPlayer.y += dy;

            my = (dy > 0 ? (gPlayer.y + gPlayer.h) : gPlayer.y);
            my /= ESize;

            mx = (gPlayer.x + Stage.scrollingOffset) / ESize;

            hit = false;

            if (Stage.Map[my][mx] != 0)
            {
                hit = true;
            }

            mx = (gPlayer.x + gPlayer.w + Stage.scrollingOffset - 1) / ESize;

            if (Stage.Map[my][mx] != 0)
            {
                hit= true;
            }

            if (hit)
            {
                adj = (dy > 0 ? -gPlayer.h : gPlayer.h);

                gPlayer.y = (my * ESize) + adj;

                isOnGround = (dy > 0);

                dy = 0;
            }
        }
    }

    void Move(MAP &Stage, LTexture &gPlayer)
    {
        dy += gravity;

        isOnGround = false;

        InScreen(gPlayer);
        Interact(Stage, gPlayer);

        if (dy != 0) gPlayer.currentFrame = 4;

        dx = 0;
    }
};

#endif // CHAR_H_INCLUDED
