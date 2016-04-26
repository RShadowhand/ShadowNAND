#include "screen.h"
#include "types.h"
#include "font.h"
#include "i2c.h"

#define SCREEN_SIZE	400 * 240 * 3 / 4 //yes I know this is more than the size of the bootom screen
#define SCREEN_TOP_WIDTH 400
#define SCREEN_TOP_HEIGHT 240

static const struct fb {
    u8 *top_left;
    u8 *top_right;
    u8 *bottom;
} *const fb = (struct fb *)0x23FFFE00;

void drawCharacter(char character, int posX, int posY, u32 color)
{
    u8 *const select = fb->top_left;

    for(int y = 0; y < 8; y++)
    {
        char charPos = font[character * 8 + y];

        for(int x = 7; x >= 0; x--)
            if ((charPos >> x) & 1)
            {
                int screenPos = (posX * SCREEN_TOP_HEIGHT * 3 + (SCREEN_TOP_HEIGHT - y - posY - 1) * 3) + (7 - x) * 3 * SCREEN_TOP_HEIGHT;

                select[screenPos] = color >> 16;
                select[screenPos + 1] = color >> 8;
                select[screenPos + 2] = color;
            }
    }
}

int drawString(const char *string, int posX, int posY, u32 color)
{
    for(int i = 0, line_i = 0; i < strlen(string); i++, line_i++)
    {
        if(string[i] == '\n')
        {
            posY += SPACING_Y;
            line_i = 0;
            i++;
        }
        else if(line_i >= (SCREEN_TOP_WIDTH - posX) / SPACING_X)
        {
            // Make sure we never get out of the screen.
            posY += SPACING_Y;
            line_i = 2; //Little offset so we know the same string continues.
            if(string[i] == ' ') i++; //Spaces at the start look weird
        }

        drawCharacter(string[i], posX + line_i * SPACING_X, posY, color);
    }

    return posY;
}

void setFramebuffers()
{
    //Gateway
    *(vu32*)0x80FFFC0 = 0x18300000;  // framebuffer 1 top left
    *(vu32*)0x80FFFC4 = 0x18300000;  // framebuffer 2 top left
    *(vu32*)0x80FFFC8 = 0x18300000;  // framebuffer 1 top right
    *(vu32*)0x80FFFCC = 0x18300000;  // framebuffer 2 top right
    *(vu32*)0x80FFFD0 = 0x18346500;  // framebuffer 1 bottom
    *(vu32*)0x80FFFD4 = 0x18346500;  // framebuffer 2 bottom
    *(vu32*)0x80FFFD8 = 1;  // framebuffer select top
    *(vu32*)0x80FFFDC = 1;  // framebuffer select bottom

    //CakeBrah
    *(vu32*)0x23FFFE00 = 0x18300000;
    *(vu32*)0x23FFFE04 = 0x18300000;
    *(vu32*)0x23FFFE08 = 0x18346500;
}

void clearScreens()
{
    for(u32 i = 0; i < (SCREEN_SIZE); i++)
    {
        *((vu32*)0x18300000 + i) = 0;
        *((vu32*)0x18346500 + i) = 0;
    }
}

void turnOnBacklight()
{
    i2cWriteRegister(3, 0x22, 0x2A); // 0x2A -> boot into firm with no backlight
}