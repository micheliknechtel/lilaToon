#ifndef __THRESHOLD__
#define __THRESHOLD__
#include "stdafx.h"
#include "region.h"
#include "histogram.h"
#include "custom_open_cv.h"

IplImage *thresholdImage(IplImage *imagePhong, IplImage *imageColor, map<int, struct Region> &regions, int numberRegions, float percentil, int increase_pixel, bool withBorder);

#endif