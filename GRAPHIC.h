#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include "defs.h"
#include "Char.h"
#include "Map.h"
#include "LTexture.h"
#include "BUTTON.h"
#include "TIME.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
SDL_Color textColor[2] = {{255, 255, 255},
                          {135, 206, 235}};
TTF_Font* gFont = nullptr;
Mix_Music* gMusic = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gGameOver = nullptr;

Time fps_time;

const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

bool Menu = true;
bool Setting = false;

int Score = 0;
std::string HighScore;

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

    if (TTF_Init() == -1)
    {
        logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                         TTF_GetError());
    }
}

TTF_Font* loadFont(const char* path, int size)
{
    TTF_Font* gFont = TTF_OpenFont( path, size );
    if (gFont == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
    }
    return gFont;
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

void GetHighScore(std::string &HighScore)
{
    std::ifstream file("data/High_Score.txt");

    file >> HighScore;

    file.close();

    return;
}

void UpdateHighScore(int score)
{
    int oldScore = 0;
    std::ifstream inFile("data/High_Score.txt");
    if (inFile) inFile >> oldScore;
    inFile.close();

    if (score > oldScore) {
        std::ofstream outFile("data/High_Score.txt", std::ios::trunc);
        if (outFile) outFile << score;
        outFile.close();
    }
}

void renderButton(const LTexture &button)
{
    SDL_Rect dest = {button.x, button.y, button.w, button.h};

    SDL_RenderCopy(renderer, button.texture, NULL, &dest);
}

void renderBackground(Background &background, LTexture &gBackground)
{
    renderTexture(gBackground.texture, background.scrollingOffset, 0);
    renderTexture(gBackground.texture, background.scrollingOffset - gBackground.w, 0);
    background.scroll(scroll, gBackground);
}

void renderExplosion(LTexture &texture)
{
    for (int i = 0 ; i < MAP_HEIGHT ; i++)
    {
        renderTexture(texture.texture, -32, i * ESize - 32, ESize * 2, ESize * 3);
    }
}

void renderChar(LTexture &gPlayer)
{
    SDL_Rect dest = {gPlayer.x, gPlayer.y, gPlayer.w, gPlayer.h};
    SDL_Rect *src = &sPlayer[gPlayer.currentFrame];

    SDL_RenderCopyEx(renderer, gPlayer.texture, src, &dest, NULL, NULL, gPlayer.flip);
    gPlayer.x -= scroll;
}

void renderMap(MAP& Stage, LTexture& gBrick, LTexture& gSpike) {
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = Stage.scrollingOffset / ESize; j <= MAP_WIDTH + Stage.scrollingOffset / ESize; j++)
        {
            int tile = Stage.Map[i][j];
            int x = j * ESize - Stage.scrollingOffset;
            int y = i * ESize;
            if (tile == 1) renderTexture(gBrick.texture, x, y, gBrick.h, gBrick.w);
            else if (tile == 2) renderTexture(gSpike.texture, x, y, gSpike.h, gSpike.w);
        }
    }

    Stage.scrollingOffset += scroll;
    while (Stage.scrollingOffset >= ESize * Stage.cnt) Stage.cnt++;

    if (Stage.cnt >= MAP_WIDTH + 1) {
        int random;
        do { random = rand() % 10; } while (random == Stage.tmp);
        Stage.tmp = random;
        Stage.loadMap(Stage.files[random]);
        Stage.scrollingOffset -= Stage.cnt * ESize;
        Stage.cnt -= MAP_WIDTH + 1;
    }
}

void DrawThickRect(SDL_Renderer* renderer, SDL_Rect rect, int thickness, const int Fill)
{
    for (int i = 0; i < thickness; i++)
    {
        SDL_Rect temp = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
        if (i == thickness - 1)
        {
            if (Fill == 1) SDL_RenderFillRect(renderer, &temp);
            else SDL_RenderDrawRect(renderer, &temp);
        }
        else SDL_RenderDrawRect(renderer, &temp);
    }
}

void HandleMusicButton(Button &button, LTexture &texture)
{
    if (button.IsInside(texture, 2))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            button.currentFrame = (button.currentFrame + 1) % 2;
            if (button.currentFrame == 1)
            {
                Mix_VolumeMusic(0);
            }
            else
            {
                Mix_VolumeMusic(128);
            }
        }
    }
}

void HandleSoundButton(Button &button, LTexture &texture)
{
    if (button.IsInside(texture, 2))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            button.currentFrame = (button.currentFrame + 1) % 2;
            if (button.currentFrame == 1)
            {
                Mix_VolumeChunk(gJump, 0);
                Mix_VolumeChunk(gGameOver, 0);
            }
            else
            {
                Mix_VolumeChunk(gJump, 128);
                Mix_VolumeChunk(gGameOver, 128);
            }
        }
    }
}

void HandlePlayButton(Button &button, LTexture &texture)
{
    button.currentFrame = 0;
    if (button.IsInside(texture, 2))
    {
        button.currentFrame = 1;
        if (event.type == SDL_MOUSEBUTTONDOWN) Menu = false;
    }
}

void HandleContinueButton(Button &button, LTexture &texture)
{
    button.currentFrame = 0;
    if (button.IsInside(texture, 2))
    {
        button.currentFrame = 1;
        if (event.type == SDL_MOUSEBUTTONDOWN) fps_time.UnPause();
    }
}

void HandleBackButton(Button &button, LTexture &texture)
{
    button.currentFrame = 0;
    if (button.IsInside(texture, 2))
    {
        button.currentFrame = 1;
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            Menu = true;
            Setting = false;
            fps_time.UnPause();
        }
    }
}

void HandleSettingButton(Button &button, LTexture &texture)
{
    button.currentFrame = 0;
    if (button.IsInside(texture, 2))
    {
        button.currentFrame = 1;
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            Setting = true;
        }
    }
}

bool HandleExitButton(Button &button, LTexture &texture)
{
    button.currentFrame = 0;
    if (button.IsInside(texture, 2))
    {
        button.currentFrame = 1;
        if (event.type == SDL_MOUSEBUTTONDOWN) return true;
    }

    return false;
}

void gameOver()
{
    Menu = true;
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
    }

    return true;
}

void quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

#endif // GRAPHIC_H_INCLUDED
