#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED

#include "defs.h"

struct Ltexture
{
    SDL_Texture* texture;

    SDL_RendererFlip flip;

    float x, y;
    int h, w;

    Ltexture()
    {
        h = ESize;
        w = ESize;
    }
};

#endif // LTEXTURE_H_INCLUDED
