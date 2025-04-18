#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "defs.h"
#include "Ltexture.h"

struct Button
{
    int x, y;

    Button(int _x = 0, int _y = 0)
    {
        x = _x;
        y = _y;
    }

    void GetButton(LTexture &button)
    {
        button.x = x;
        button.y = y;
        button.h = COMMON_BUTTON_HEIGHT;
        button.w = COMMON_BUTTON_WIDTH;
    }

    bool IsInside(const LTexture &button, const int Size)
    {
        int X, Y;

        SDL_GetMouseState(&X, &Y);

		int button_width, button_height;
		if (Size == SMALL_BUTTON)
		{
			button_width = SMALL_BUTTON_WIDTH;
			button_height = SMALL_BUTTON_HEIGHT;
		}
		else
		{
			button_width = COMMON_BUTTON_WIDTH;
			button_height = COMMON_BUTTON_HEIGHT;
		}
        if (X >= x && X <= x + button_width && Y >= y && Y <= y + button_height)
        {
            return true;
        }
        return false;
    }
};

#endif // BUTTON_H_INCLUDED
