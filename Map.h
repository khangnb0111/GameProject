#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <fstream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>

#include "defs.h"
#include "GRAPHIC.h"

struct MAP
{
    SDL_Texture *texture;

    SDL_Rect dest;

    int Map[MAP_HEIGHT][(MAP_WIDTH + 1) * 2] = {0};
    int MapPre[MAP_HEIGHT][MAP_WIDTH] = {0};
    int scrollingOffset = 0;
    int tmp;

    std::string files[10];

    MAP()
    {
        for (int i = 0; i < 10; i++)
        {
            files[i] = "data/Level-";
            files[i] += char(i + '0');
            files[i] += ".txt";
        }

        dest.w = ESize;
        dest.h = ESize;

        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j <= MAP_WIDTH; j++)
            {
                if (i == 0 || i == MAP_HEIGHT - 1)
                {
                    Map[i][j] = 1;
                }
                else
                {
                    Map[i][j] = 0;
                }
            }
        }

        Map[MAP_HEIGHT - 1][MAP_WIDTH] = 0;

        tmp = rand() % 10;

        loadMap(files[tmp]);
    }

    void loadMap(const std::string &fileName)
    {
        std::ifstream file(fileName);

        if (!file)
        {
            std::cout << "Error opening file: " << fileName << std::endl;
            return;
        }

        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = MAP_WIDTH + 1; j < (MAP_WIDTH + 1) * 2; j++)
            {
                if (!(file >> Map[i][j]))
                {
                    std::cout << "Error reading map data" << std::endl;
                    return;
                }
            }
        }

        file.close();
    }

    void MapMove()
    {
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < 2 * (MAP_WIDTH + 1) - 1; j++)
            {
                Map[i][j] = Map[i][j + 1];
            }
        }
    }
};

struct Background
{
    SDL_Texture *texture;

    SDL_Rect dest;

    int scrollingOffset = 0;

    void setTexture()
    {
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    }

    void scroll(int distance)
    {
        scrollingOffset -= distance;
        if( scrollingOffset < 0 ) { scrollingOffset = dest.w; }
    }

};

#endif // MAP_H_INCLUDED
