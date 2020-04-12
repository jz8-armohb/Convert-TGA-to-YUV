#include <iostream>
#include "declarations.h"

int rgb66[256], rgb129[256], rgb25[256];
int rgb38[256], rgb74[256], rgb112[256];
int rgb94[256], rgb18[256];

void rgbLookupTable()
{
	for (int i = 0; i < 256; i++)
	{
		rgb66[i] = 66 * i;
		rgb129[i] = 129 * i;
		rgb25[i] = 25 * i;
		rgb38[i] = 38 * i;
		rgb74[i] = 74 * i;
		rgb112[i] = 112 * i;
		rgb94[i] = 94 * i;
		rgb18[i] = 18 * i;
	}
}

void rgb2yuv(FILE* yuvPtr, int width, int height, unsigned char* rgbBuff)
{
	int pxCount = width * height;
	unsigned char* yBuff = new unsigned char[pxCount];		// Buffer for Y component
	unsigned char* uBuff = new unsigned char[pxCount / 4];	// Buffer for U component
	unsigned char* vBuff = new unsigned char[pxCount / 4];	// Buffer for V component
	unsigned char* uBuff444 = new unsigned char[pxCount];	// Buffer for U component in 4:4:4 format
	unsigned char* vBuff444 = new unsigned char[pxCount];	// Buffer for V component in 4:4:4 format

	// RGB to YUV (4:4:4)
	for (int i = 0; i < pxCount; i++)	// i for pixel number
	{
		unsigned char r = rgbBuff[3 * i + 2];	// R component of the ith pixel
		unsigned char g = rgbBuff[3 * i + 1];	// G component of the ith pixel
		unsigned char b = rgbBuff[3 * i];		// B component of the ith pixel
		rgbLookupTable();
		yBuff[i] = ((rgb66[r] + rgb129[g] + rgb25[b]) >> 8) + 16;
		uBuff444[i] = ((-rgb38[r] - rgb74[g] + rgb112[b]) >> 8) + 128;
		vBuff444[i] = ((rgb112[r] - rgb94[g] - rgb18[b]) >> 8) + 128;
	}

	// 4:4:4 to 4:2:0
	for (int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j += 2)
		{
			uBuff[i / 2 * width / 2 + j / 2] = uBuff444[i * width + j];
			vBuff[i / 2 * width / 2 + j / 2] = vBuff444[i * width + j];
		}
	}
	delete[]uBuff444;
	delete[]vBuff444;

	fwrite(yBuff, sizeof(unsigned char), pxCount, yuvPtr);
	fwrite(uBuff, sizeof(unsigned char), pxCount / 4, yuvPtr);
	fwrite(vBuff, sizeof(unsigned char), pxCount / 4, yuvPtr);
}