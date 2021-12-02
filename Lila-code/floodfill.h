#ifndef __FLOODFILL__
#define __FLOODFILL__

#include "stdafx.h"
#include "custom_open_cv.h"
#include "region.h"
#define PIXEL_LIMIT 255
#define REGION_SIZE 16581375

int doFloodFill(IplImage *image, map<int, struct Region> &regions);
int doFloodFillNewRegion(IplImage *image, int x, int y, map<int, struct Region> &regions);

#endif
