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

    void loadTexture(const char *filename, SDL_Renderer* renderer)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);

        SDL_Texture* newTexture = IMG_LoadTexture(renderer, filename);
        if (newTexture == NULL) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"Load texture %s", IMG_GetError());
        }

        texture = newTexture;
    }

    void loadText(std::string text, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
            return;
        }

        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
        }

        SDL_FreeSurface( textSurface );
        return;
    }
};

#endif // LTexture_H_INCLUDED
