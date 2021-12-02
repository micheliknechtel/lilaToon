#ifndef __PHONG__
#define __PHONG__

#include "stdafx.h"
#include <stdio.h>
#include "custom_open_cv.h"

struct point
{
	float x;
	float y;
	float z;
};

void Phong(int iR, int iG, int iB,  float x, float y, float z, int pX, int pY, float shine, point light, point centerCanvas, float *Ipr, float *Ipg, float *Ipb, int w, int h, bool enableSpecular, float ambiente);

#endif
