#include "stdio.h"
#include <iostream>
#include "stdlib.h"
#include "math.h"
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[])
{
    /* Declarations */
    FILE* tgaFilePtr, * yuvFilePtr;
    //FILE* rgbFilePtr;
    HEADER hd;  // Structure variable for TGA file header
    int w, h, pxCount;
    const char* tgaFileName = "snow32RLE.tga";
    const char* yuvFileName = "snow32RLE.yuv";
    //const char* rgbFileName = "snow16.rgb";
    PIXEL* rgbaData = NULL;  // Entire RGBA data of TGA file; used for future funtions
    unsigned char* rgbBuffer = NULL; // RGB data of TGA file (in .rgb format); extracted from rgbaData; used for tga2yuv
    int offset = 0;

    /* Open the files */
    if (fopen_s(&tgaFilePtr, tgaFileName, "rb") == 0)
    {
        cout << "Successfully opened \"" << tgaFileName << "\".\n";
    }
    else
    {
        cout << "Failed to open \"" << tgaFileName << "\".\n";
        exit(-1);
    }
    if (fopen_s(&yuvFilePtr, yuvFileName, "wb") == 0)
    {
        cout << "Successfully opened \"" << yuvFileName << "\".\n";
    }
    else
    {
        cout << "Failed to open \"" << yuvFileName << "\".\n";
        exit(-1);
    }
    //if (fopen_s(&rgbFilePtr, rgbFileName, "wb") == 0)
    //{
    //    cout << "Successfully opened \"" << rgbFileName << "\".\n";
    //}
    //else
    //{
    //    cout << "Failed to open \"" << rgbFileName << "\".\n";
    //    exit(-1);
    //}



    /* Read and display the header fields */
    ReadTgaHeader(&hd, tgaFilePtr);
    w = hd.height;
    h = hd.width;
    pxCount = w * h;

    /* Space allocation */
    rgbaData = new PIXEL[hd.width * hd.height];
    memset(rgbaData, 0, hd.height * hd.width);   // Initialisation
    rgbBuffer = new unsigned char[hd.width * hd.height * 3];
    memset(rgbBuffer, 0, hd.height * hd.width * 3);   // Initialisation

    /* Developed function check & invalidation check */
    if (hd.imageTypeCode != 2 && hd.imageTypeCode != 10)
    {
        cout << "Can only handle image type 2 (uncompressed, unmapped RGB) & image type 10 (run length encoded, unmapped RGB).\nOther options being developed.\n";
        exit(-1);
    }
    if (hd.bitsPerPixel != 16 && hd.bitsPerPixel != 24 && hd.bitsPerPixel != 32)
    {
        cout << "Invalid value of image pixel size!\nCan only handle pixel depths of 16, 24, and 32.\n";
        exit(-1);
    }
    if (hd.colourMapType != 0 && hd.colourMapType != 1)
    {
        cout << "Invalid value of colour map type!\nCan only handle colour map types of 0 and 1.\n";
        exit(-1);
    }

    /* Skip over unnecessary chunks */
    offset += hd.idLength;
    offset += hd.colourMapType * hd.colourMapLength * hd.colourMapDepth;  // ????应该还要乘hd.colourMapDepth吧？
    cout << offset << " byte(s) skipped over.\n\n";
    fseek(tgaFilePtr, offset, SEEK_CUR);  // Skip 'offset' bytes from the end of header

    /* Read the image RGB (A, if exists) data */
    ReadColourData(&hd, rgbaData, tgaFilePtr);

    /* Transform .tga formatted RGB data into .rgb format */
    Trans2RgbFormat(rgbaData, rgbBuffer, &hd);
    //fwrite(rgbBuffer, 3, pxCount, rgbFilePtr);

    /* Transform RGB into YUV */
    rgb2yuv(yuvFilePtr, w, h, rgbBuffer);

    delete[]rgbaData;
    delete[]rgbBuffer;
    //fclose(rgbFilePtr);
    fclose(tgaFilePtr);
    fclose(yuvFilePtr);
}