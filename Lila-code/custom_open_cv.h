#ifndef __CUSTOM_OPEN_CV__
#define __CUSTOM_OPEN_CV__
#include <stdio.h>
#include <opencv2/core.hpp>


int RGB2Int(int R, int G, int B);
void Int2RGB(int in, int *R, int *G, int *B);
void GET_PIXEL(IplImage *image, int x, int y, int *p);
void GET_PIXELRGB(IplImage *image, int x, int y, int c, int *p);
void SET_PIXEL(IplImage *image, int x, int y, int p);
void SET_PIXELRGB(IplImage *image, int x, int y, int p, int c);
void SET_PIXEL4INT(IplImage *image, int x, int y, int p);
void GET_PIXEL4INT(IplImage *image, int x, int y, int *p);

void display(char name[], IplImage *image);
IplImage * createImage(int width, int height, int depth, int channel);
IplImage * createImageRGB(int width, int height);
void freeResource(char name[], IplImage **image);

#define WHITE_COLOR 16777215
#define BLACK_COLOR 0

#endif
