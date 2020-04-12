#pragma once
#include <iostream>

typedef struct
{
    char  idLength = 0;         // Length of identification field (length = 1B, offset = 0B)
    char  colourMapType = 0;    // Colour map type (length = 1B, offset = 1B): 0 for no colour map included, 1 for colour map included
    char  imageTypeCode = 0;    // Image type code (length = 1B, offset = 2B): 2 for uncompressed & unmapped RGB image, 10 for run length encoded & unmapped RGB image

    /* Colour map specification (all set to 0 if colour map type is 0) */
    short colourMapOffset = 0;  // Colour map origin (length = 2B, offset = 3B): index of first colour map entry
    short colourMapLength = 0;  // Colour map length (length = 2B, offset = 5B): number of colour map entries
    char  colourMapDepth = 0;   // Colour map depth (length = 1B, offfset = 7B): number of bits in each entry. 16 for the Targa 16, 24 for the Targa 24, 32 for the Targa 32

    /* Image specification */
    short x_origin = 0;         // X coordinate of the lower left corner (length = 2B, offset = 8B)
    short y_origin = 0;         // Y coordinate of the lower left corner (length = 2B, offset = 10B)
    short width = 0;            // Width of image (length = 2B, offset = 12B)
    short height = 0;           // Height of image (length = 2B, offset = 14B)
    char  bitsPerPixel = 0;     // Number of bits in each pixel (length = 1B, offset = 16B)
    char  imageDescriptor = 0;  // Image descripter byte (length = 1B, offset = 17B)
} HEADER;

typedef struct
{
    unsigned char r, g, b, a;
} PIXEL;

void ReadTgaHeader(HEADER* tgaHdPtr, FILE* tgaPtr);
void ReadColourData(HEADER* tgaHdPtr, PIXEL* colourData, FILE* tgaPtr);
void Trans2RgbFormat(PIXEL* colourData, unsigned char* rgbBuff, HEADER* tgaHdPtr);
void rgb2yuv(FILE* yuvPtr, int width, int height, unsigned char* rgbBuff);