#ifndef __CHAINCODE__
#define __CHAINCODE__

#include "stdafx.h"

IplImage *ChainCode(IplImage *image,
					int numberRegions,
					map<int, struct Region> &regions);

#endif
