#include <iostream>
#include "declarations.h"
using namespace std;

void ReadTgaHeader(HEADER* tgaHdPtr, FILE* tgaPtr)
{
    /* Read data in TGA file header fields */
    tgaHdPtr->idLength = fgetc(tgaPtr);
    tgaHdPtr->colourMapType = fgetc(tgaPtr);
    tgaHdPtr->imageTypeCode = fgetc(tgaPtr);
    fread(&tgaHdPtr->colourMapOffset, 2, 1, tgaPtr);
    fread(&tgaHdPtr->colourMapLength, 2, 1, tgaPtr);
    tgaHdPtr->colourMapDepth = fgetc(tgaPtr);
    fread(&tgaHdPtr->x_origin, 2, 1, tgaPtr);
    fread(&tgaHdPtr->y_origin, 2, 1, tgaPtr);
    fread(&tgaHdPtr->width, 2, 1, tgaPtr);
    fread(&tgaHdPtr->height, 2, 1, tgaPtr);
    tgaHdPtr->bitsPerPixel = fgetc(tgaPtr);
    tgaHdPtr->imageDescriptor = fgetc(tgaPtr);

    /* Display header info on screen */
    cout << "\nTGA file header information:\n";
    printf("ID length:         %d\n", tgaHdPtr->idLength);
    printf("Colour Map type:   %d\n", tgaHdPtr->colourMapType);
    printf("Image type code:   %d\n", tgaHdPtr->imageTypeCode);
    printf("Colour map offset: %d\n", tgaHdPtr->colourMapOffset);
    printf("Colour map length: %d\n", tgaHdPtr->colourMapLength);
    printf("Colour map depth:  %d\n", tgaHdPtr->colourMapDepth);
    printf("X origin:          %d\n", tgaHdPtr->x_origin);
    printf("Y origin:          %d\n", tgaHdPtr->y_origin);
    printf("Width:             %d\n", tgaHdPtr->width);
    printf("Height:            %d\n", tgaHdPtr->height);
    printf("Image pixel size:  %d\n", tgaHdPtr->bitsPerPixel);
    printf("Descriptor:        %d\n\n", tgaHdPtr->imageDescriptor);
}