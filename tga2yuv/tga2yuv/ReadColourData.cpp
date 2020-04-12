#include "declarations.h"

void SeparateRGBA(PIXEL* pixel, unsigned char* rgba, int bytesPerPixel)
{
    switch (bytesPerPixel)
    {
    case 4:
        pixel->r = rgba[2];
        pixel->g = rgba[1];
        pixel->b = rgba[0];
        pixel->a = rgba[3];
        break;

    case 3:
        pixel->r = rgba[2];
        pixel->g = rgba[1];
        pixel->b = rgba[0];
        pixel->a = 255;
        break;

    case 2:
        pixel->r = (rgba[1] & 0b01111100) << 1;
        pixel->g = ((rgba[1] & 0b00000011) << 6) | ((rgba[0] & 0b11100000) >> 2);
        pixel->b = (rgba[0] & 0b00011111) << 3;
        pixel->a = (rgba[1] & 0b10000000);
        break;

    default:
        break;
    }
}

void ReadColourData(HEADER* tgaHdPtr, PIXEL* colourData, FILE* tgaPtr)
{
    int bytesPerPx = tgaHdPtr->bitsPerPixel / 8;    // Bytes per pixel
    unsigned char tempRGBA[4];  // Temporary buffer for the RGBA data of 1 pixel

    int n = 0;  // nth pixel
    while (n < tgaHdPtr->width * tgaHdPtr->height)
    {
        switch (tgaHdPtr->imageTypeCode)
        {
        /* Uncompressed, unmapped RGB image */
        case 2:
        {
            /* Read the colour data of 1 pixel */
            if (fread(tempRGBA, 1, bytesPerPx, tgaPtr) != bytesPerPx)
            {
                printf("ERROR!!! Unexpected end of file at pixel %d.\n", n);
                exit(-1);
            }

            SeparateRGBA(&(colourData[n]), tempRGBA, bytesPerPx);
            //printf("%-4x%-4x%-4x\n", colourData[n].b, colourData[n].g, colourData[n].r);   // Check
            n++;
            break;
        }


        /* Run length encoded, unmapped RGB image */
        case 10:
        {
            unsigned char tempPktHeader;    // Temporary buffer for the packet header
            int pktHdID = 0;    // Determines if it's an RL packet or a raw packet 
            int pktRunSize = 0;    // Run size (the number of pixels in this packet)

            /* Read the packet header */
            if (fread(&tempPktHeader, 1, 1, tgaPtr) != 1)
            {
                printf("ERROR!!! Unexpected end of file at pixel %d.\n", n);
                exit(-1);
            }
            pktHdID = (tempPktHeader & 0x80) >> 7;
            pktRunSize = (tempPktHeader & 0x7F) + 1;

            /* Raw packet */
            if (pktHdID == 0)
            {
                for (int i = 0; i < pktRunSize; i++)
                {
                    if (fread(tempRGBA, 1, bytesPerPx, tgaPtr) != bytesPerPx)
                    {
                        printf("ERROR!!! Unexpected end of file at pixel %d.\n", n);
                        exit(-1);
                    }
                    SeparateRGBA(&(colourData[n]), tempRGBA, bytesPerPx);
                    n++;
                }
            }
            /* RL packet */
            else if (pktHdID == 1)
            {
                if (fread(tempRGBA, 1, bytesPerPx, tgaPtr) != bytesPerPx)
                {
                    printf("ERROR!!! Unexpected end of file at pixel %d.\n", n);
                    exit(-1);
                }
                for (int i = 0; i < pktRunSize; i++)
                {
                    SeparateRGBA(&(colourData[n]), tempRGBA, bytesPerPx);
                    n++;
                }
            }
            else
            {
                printf("ERROR!!! Unexpected invalid value of packet header ID\n");
                exit(-1);
            }

            break;
        }


        default:
            break;
        }
    }
}