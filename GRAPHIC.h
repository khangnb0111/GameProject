#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>

#include "defs.h"
#include "Char.h"

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

    void renderWall(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect
    }

    void initImage()
    {
        pos = loadTexture("Image/ninja.png");
        dirt = loadTexture("Image/dirt.jpg");
        //shuriken = loadTexture("Image/ninja.png");
    }

    void render(Player &player)
    {
        SDL_RenderClear(renderer);

        renderChar(pos, player.dest, player.flip);
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

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // GRAPHIC_H_INCLUDED
