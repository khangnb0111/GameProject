#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include "defs.h"
#include "Char.h"
#include "Map.h"
#include "Ltexture.h"

struct Menu
{
    SDL_Texture* startButton;
};

struct Game
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    bool start = false;

    int x, y;

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

    SDL_Texture* loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,"Load texture %s", IMG_GetError());
        }

        return texture;
    }

    void initImage(Ltexture &gBackground, Player &player, Ltexture &dirt, Menu &menu)
    {
        player.texture = loadTexture("Image/ninja.png");
        dirt.texture = loadTexture("Image/dirt.jpg");
        gBackground.texture = loadTexture("Image/forest.jpg");
        menu.startButton = loadTexture("Image/startbutton.png");
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

    void init(Ltexture &gBackground, Player &player, Ltexture &dirt, Menu &menu)
    {
        initSDL();
        initImage(gBackground, player, dirt, menu);
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

    void rendermenu(Menu &menu)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        renderTexture(menu.startButton, 576, 352, 64, 128);
    }

    void renderBackground(Background &background, Ltexture &gBackground)
    {
        renderTexture(gBackground.texture, background.scrollingOffset, 0);
        renderTexture(gBackground.texture, background.scrollingOffset - gBackground.w, 0);
        background.scroll(scroll, gBackground);
    }

    void renderChar(Player &player)
    {
        SDL_RenderCopyEx(renderer, player.texture, NULL, &player.dest, NULL, NULL, player.flip);
        player.dest.x -= scroll;
    }

    void renderStage(MAP &Stage, Ltexture &dirt)
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

    void render(Player &player, MAP &Stage, Background &background, Ltexture &dirt, Ltexture &gBackground)
    {
        SDL_RenderClear(renderer);

        renderBackground(background, gBackground);
        renderChar(player);
        renderStage(Stage, dirt);
    }

    void gameOver(Player &player, MAP &Stage)
    {
        if (player.dest.x <= ESize || player.dest.y >= SCREEN_HEIGHT - ESize)
        {
            start = false;
            player.reset();
            Stage.reset();
            return;
        }
        return;
    }

    bool running(Player &player)
    {
        SDL_GetMouseState(&x, &y);

        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (x > 576 && x < 704 && y > 352 && y < 416) start = true;
                break;
        }
        return true;
    }

    void quit(Player &player, Menu &menu, Ltexture &dirt, Ltexture &gBackground)
    {
        IMG_Quit();

        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;

        SDL_DestroyTexture(dirt.texture);
        dirt.texture = nullptr;

        SDL_DestroyTexture(gBackground.texture);
        gBackground.texture = nullptr;

        SDL_DestroyTexture(menu.startButton);
        menu.startButton = nullptr;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // GRAPHIC_H_INCLUDED
