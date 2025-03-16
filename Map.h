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

    MAP()
    {
        dest.w = ESize;
        dest.h = ESize;

        loadMap("data/Level-1.txt", "data/Level-1.txt");
    }

    void loadMap(const std::string &fileName1, const std::string &fileName2)
    {
        std::ifstream file1(fileName1);
        if (!file1)
        {
            std::cout << "Error opening file: " << fileName1 << std::endl;
            return;
        }

        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH + 1; j++)
            {
                if (!(file1 >> Map[i][j]))
                {
                    std::cout << "Error reading map data" << std::endl;
                    return;
                }
            }
        }

        file1.close();

        std::ifstream file2(fileName2);

        if (!file2)
        {
            std::cout << "Error opening file: " << fileName2 << std::endl;
            return;
        }

        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = MAP_WIDTH + 1; j < (MAP_WIDTH + 1) * 2; j++)
            {
                if (!(file2 >> Map[i][j]))
                {
                    std::cout << "Error reading map data" << std::endl;
                    return;
                }
            }
        }

        file2.close();
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
