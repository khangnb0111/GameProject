#ifndef LTexture_H_INCLUDED
#define LTexture_H_INCLUDED

#include "defs.h"

struct LTexture
{
    SDL_Texture* texture = nullptr;

    SDL_RendererFlip flip;

    float x, y;
    int h, w;

    int currentFrame = 0;

    LTexture()
    {
        x = SCREEN_WIDTH / 2;
        y = SCREEN_HEIGHT / 2;
        h = ESize;
        w = ESize;
    }

    ~LTexture()
    {
        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    SDL_Texture* loadTexture(const char *filename, SDL_Renderer* renderer)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);

        SDL_Texture* newTexture = IMG_LoadTexture(renderer, filename);
        if (newTexture == NULL) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"Load texture %s", IMG_GetError());
        }

        texture = newTexture;
        return texture;
    }
};

#endif // LTexture_H_INCLUDED
