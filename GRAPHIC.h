#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include "defs.h"
#include "Char.h"
#include "Map.h"
#include "LTexture.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
SDL_Color textColor = { 0, 0, 0 };
TTF_Font* gFont = nullptr;
Mix_Music* gMusic = nullptr;
Mix_Music* gMenuMusic = nullptr;
Mix_Chunk* gClick = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;

const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

bool Menu = true;
bool Continue = true;

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

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
}

Mix_Music *loadMusic(const char* path)
{
    Mix_Music *gMusic = Mix_LoadMUS(path);
    if (gMusic == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
            "Could not load music! SDL_mixer Error: %s", Mix_GetError());
    }
    return gMusic;
}

Mix_Chunk* loadSound(const char* path)
{
    Mix_Chunk* gChunk = Mix_LoadWAV(path);
    if (gChunk == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
    }
    return gChunk;
}

void play(Mix_Chunk* gChunk)
{
    if (gChunk != nullptr) {
        Mix_PlayChannel( -1, gChunk, 0 );
    }
}

void playMusic(Mix_Music *gMusic)
{
    if (gMusic == nullptr) return;

    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic( gMusic, -1 );
    }
    else if( Mix_PausedMusic() == 1 ) {
        Mix_ResumeMusic();
    }
}

void present()
{
    SDL_RenderPresent(renderer);
}

void renderTexture(SDL_Texture *texture, int x, int y, int h = SCREEN_HEIGHT, int w = SCREEN_WIDTH)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void renderButton(const LTexture &button)
{
    SDL_Rect dest = {button.x, button.y, button.w, button.h};
    SDL_Rect* src = &sButton[button.currentFrame];

    SDL_RenderCopy(renderer, button.texture, src, &dest);
}

void renderBackground(Background &background, LTexture &gBackground)
{
    renderTexture(gBackground.texture, background.scrollingOffset, 0);
    renderTexture(gBackground.texture, background.scrollingOffset - gBackground.w, 0);
    background.scroll(scroll, gBackground);
}

void renderChar(LTexture &gPlayer)
{
    SDL_Rect dest = {gPlayer.x, gPlayer.y, gPlayer.w, gPlayer.h};
    SDL_Rect *src = &sPlayer[gPlayer.currentFrame];

    SDL_RenderCopyEx(renderer, gPlayer.texture, src, &dest, NULL, NULL, gPlayer.flip);
    gPlayer.x -= scroll;
}

void renderMap(MAP &Stage, LTexture &dirt)
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH + 1; j++)
        {
            if (Stage.Map[i][j] == 1)
            {
                dirt.x = j * ESize - (Stage.scrollingOffset % ESize);
                dirt.y = i * ESize;
                renderTexture(dirt.texture, dirt.x, dirt.y, dirt.h, dirt.w);
            }
        }
    }

    Stage.scrollingOffset += scroll;

    if (Stage.scrollingOffset % ESize == 0) Stage.MapMove();
    if (Stage.scrollingOffset >= SCREEN_WIDTH + ESize)
    {
        int random = rand() % 10;
        while (random == Stage.tmp)
        {
            random = rand() % 10;
        }
        Stage.tmp = random;
        Stage.loadMap(Stage.files[random]);
        Stage.scrollingOffset = 0;
    }
}

void renderGame(LTexture &gPlayer, MAP &Stage, Background &background, LTexture &dirt, LTexture &gBackground)
{
    SDL_RenderClear(renderer);

    renderBackground(background, gBackground);
    renderChar(gPlayer);
    renderMap(Stage, dirt);
}

void gameOver(Player &player, MAP &Stage, LTexture &gPlayer)
{
    int x, y;

    SDL_GetMouseState(&x, &y);

    if (gPlayer.x <= ESize || gPlayer.y >= SCREEN_HEIGHT - ESize)
    {
        Menu = true;
        player.reset(gPlayer);
        Stage.reset();
        return;
    }
    return;
}

bool running()
{
    int x, y;

    SDL_GetMouseState(&x, &y);

    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            return false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (Menu && x > 576 && x < 704 && y > 352 && y < 416) Menu = false;
            break;
    }

    return true;
}

void quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

#endif // GRAPHIC_H_INCLUDED
