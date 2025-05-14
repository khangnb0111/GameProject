#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include "defs.h"

struct Time
{
    int startTick;
    int pauseTick;

    bool IsPause;
    bool IsStart;

    Time()
    {
        startTick = 0;
        pauseTick = 0;
        IsStart = false;
        IsPause = false;
    }

    void Start()
    {
        if (!IsStart) IsStart = true;
        startTick = SDL_GetTicks();
    }

    void Stop()
    {
        IsStart = false;
        IsPause = false;
    }

    void Pause()
    {
        if (IsStart && !IsPause)
        {
            IsPause = true;
            pauseTick = SDL_GetTicks() - startTick;
        }
    }

    void UnPause()
    {
        if (IsPause)
        {
            IsPause = false;
            startTick = SDL_GetTicks() - pauseTick;
            pauseTick = 0;
        }
    }

    int GetTick()
    {
        if (IsStart)
        {
            if (IsPause) return pauseTick;
            else return SDL_GetTicks() - startTick;
        }

        return 0;
    }


};

#endif // TIME_H_INCLUDED
