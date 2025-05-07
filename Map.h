#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "defs.h"
#include "Ltexture.h"

struct MAP
{
    int Map[MAP_HEIGHT][(MAP_WIDTH + 1) * 2] = {0};
    float scrollingOffset = 0;
    int cnt = 0;
    int tmp;

    std::string files[10] = {
                "data/Level-0.txt",
                "data/Level-1.txt",
                "data/Level-2.txt",
                "data/Level-3.txt",
                "data/Level-4.txt",
                "data/Level-5.txt",
                "data/Level-6.txt",
                "data/Level-7.txt",
                "data/Level-8.txt",
                "data/Level-9.txt"};

    MAP()
    {
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

    void reset()
    {
        scrollingOffset = 0;

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

    void MapScroll()
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
    float scrollingOffset = 0;

    void setTexture(LTexture &gBackground)
    {
        gBackground.w = SCREEN_WIDTH;
        gBackground.h = SCREEN_WIDTH;
    }

    void scroll(const float distance, LTexture &gBackground)
    {
        scrollingOffset -= distance;
        if( scrollingOffset < 0 ) { scrollingOffset = gBackground.w; }
    }

};

#endif // MAP_H_INCLUDED
