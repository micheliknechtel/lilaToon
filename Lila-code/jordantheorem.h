#ifndef __TREEREGIONS__
#define __TREEREGIONS__

#include "stdafx.h"

void treeRegions(int numberRegions, map<int, struct Region> &regions, int h, int w);
bool beContained(int start_x, int start_y, vector<int> &array_x, vector<int> &array_y, int h, int w);

#endif
