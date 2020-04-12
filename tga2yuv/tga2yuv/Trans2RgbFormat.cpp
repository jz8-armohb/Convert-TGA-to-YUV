#include "declarations.h"

void Trans2RgbFormat(PIXEL* colourData, unsigned char* rgbBuff, HEADER* tgaHdPtr)
{
    /* Write RGB data in .rgb format into rgbBuff */
    int w = tgaHdPtr->width;
    int h = tgaHdPtr->height;

    for (int i = 0; i < h; i++)   // i for row of image
    {
        for (int j = 0; j < w; j++) // j for column of image
        {
            int rgbPxNum = (h - 1 - i) * w + j; // Pixel number in RGB file
            int tgaPxNum = i * w + j;   // Pixel number in TGA file

            rgbBuff[3 * rgbPxNum + 2] = colourData[tgaPxNum].r;
            rgbBuff[3 * rgbPxNum + 1] = colourData[tgaPxNum].g;
            rgbBuff[3 * rgbPxNum] = colourData[tgaPxNum].b;
        }
    }
}