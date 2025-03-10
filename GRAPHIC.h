#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>

#include "defs.h"
#include "Char.h"
#include "Map.h"

struct Game
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    SDL_Texture *pos, *dirt;

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void initSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void prepare()
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
    }

    void present()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture* loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"Load texture %s", IMG_GetError());
        }

        return texture;
    }

    void renderChar(SDL_Texture *texture, SDL_Rect &dest, SDL_RendererFlip flip)
    {
        SDL_RenderCopyEx(renderer, texture, NULL, &dest, NULL, NULL, flip);
    }

    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void renderStage(SDL_Texture *texture, MAP &Stage, int n, int m)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (Stage.Map[i][j] == 1)
                {
                    Stage.dest.x = j * ESize;
                    Stage.dest.y = i * ESize;
                    SDL_RenderCopy(renderer, texture, NULL, &Stage.dest);
                }
            }
        }
    }

    void initImage()
    {
        pos = loadTexture("Image/ninja.png");
        dirt = loadTexture("Image/dirt.jpg");
    }

    void render(Player &player, MAP &Stage)
    {
        SDL_RenderClear(renderer);

        renderChar(pos, player.dest, player.flip);
        renderStage(dirt, Stage, 10, 15);
    }

    void init()
    {
        initSDL();
        initImage();
    }

    bool running()
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) return false;
        return true;
    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyTexture(pos);
        pos = nullptr;

        SDL_DestroyTexture(dirt);
        dirt = nullptr;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // GRAPHIC_H_INCLUDED
