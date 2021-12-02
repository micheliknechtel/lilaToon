#ifndef __COLORIMAGE__
#define __COLORIMAGE__

#include "stdafx.h"

IplImage *MakeColorImage(IplImage* image,
						 int numberRegions,
						 map<int, struct Region> &regions);

#endif
