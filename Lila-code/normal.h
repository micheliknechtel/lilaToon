#ifndef __NORMAL__
#define __NORMAL__

#include "stdafx.h"

float normalZscalar(float npX, float npY, int scalar);
void normalize(map<int,struct Region> &regions, int numberRegions);
void normalize_inc(struct Region &region, vector<int> vX, vector<int> vY);
void normalize_plus(struct Region &region, map<int, struct Region> &regions, int numberRegions);

#endif
