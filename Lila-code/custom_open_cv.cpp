#include "stdafx.h"
#include "custom_open_cv.h"

int RGB2Int(int R, int G, int B)
{
	int Rx, Gx, Bx;
	int ret = 0;
	Rx = (R << 16);
	Gx = G << 8;
	Bx = B;
	ret = Rx | Gx | Bx;
	return ret;
}

void Int2RGB(int in, int *R, int *G, int *B)
{
	int temp = in;
	int rest = 0;

	temp = (temp >> 16);
	*R = temp;

	temp = in - (temp << 16);
	*G = temp >> 8;

	temp = temp - (temp >> 8 << 8);
	*B = temp;
}

void GET_PIXEL(IplImage *image, int x, int y, int *p)
{
	if(x <= image->width && y <= image->height && x > -1 && y > -1)
		*p = (uchar)image->imageData[y*image->widthStep + x*image->nChannels];
	else
		*p = -1;
}
void GET_PIXELRGB(IplImage *image, int x, int y, int c, int *p)
{
	if(x <= image->width && y <= image->height && x > -1 && y > -1)
		*p = (uchar)image->imageData[y*image->widthStep + x*image->nChannels + c];
	else
		*p = -1;
}

void GET_PIXEL4INT(IplImage *image, int x, int y, int *p)
{
	int r, g, b;
	if(x <= image->width && y <= image->height && x > -1 && y > -1)
	{
		b = (uchar)image->imageData[y*image->widthStep + x*image->nChannels + 0];
		g = (uchar)image->imageData[y*image->widthStep + x*image->nChannels + 1];
		r = (uchar)image->imageData[y*image->widthStep + x*image->nChannels + 2];
	}
	else
	{
		b = -1;
		g = -1;
		r = -1;
	}
	*p = RGB2Int(r, g, b);
}

void SET_PIXEL(IplImage *image, int x, int y, int p)
{
	if(x <= image->width && y <= image->height && x > -1 && y > -1)
	{
		image->imageData[y*image->widthStep + x*image->nChannels] = p;
	}
}

void SET_PIXEL4INT(IplImage *image, int x, int y, int p)
{
	int r, g, b;
	Int2RGB(p, &r, &g, &b);
	if(x <= image->width && y <= image->height && x > -1 && y > -1)
	{
		image->imageData[y*image->widthStep + x*image->nChannels ] = b;
		image->imageData[y*image->widthStep + x*image->nChannels + 1] = g;
		image->imageData[y*image->widthStep + x*image->nChannels + 2] = r;
	}
}
void SET_PIXELRGB(IplImage *image, int x, int y, int p, int c)
{
	if(x <= image->width && y <= image->height && x > -1 && y > -1)
	{
		image->imageData[y*image->widthStep + x*image->nChannels +c] = p;
	}
}

IplImage * createImage(int width, int height, int depth, int channel)
{
	return cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, channel);
}
IplImage * createImageRGB(int width, int height)
{
	return cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
}
