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
    }

    bool IsInside(const int X, const int Y, LTexture &button, int Size)
    {
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
