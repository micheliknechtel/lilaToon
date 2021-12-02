#ifndef __RUN__
#define __RUN__

#include "stdafx.h"
#include "custom_open_cv.h"

#include "stack.h"
#include "thinning.h"
#include "floodfill.h"
#include "chaincode.h"
#include "colorimage.h"
#include "jordantheorem.h"
#include "normal.h"
#include "phong.h"
#include "threshold.h"

IplImage *processThinning(char *filename_original);
IplImage *processFloodFill(IplImage *image, struct Regions *regions);
IplImage *processChaincode(IplImage *image, struct Regions *regions);
IplImage *processMakeColor(char *filename_color,struct Regions *regions);
IplImage *processColor(char *filename_color);
IplImage *processWhite(IplImage *image);
void processJordanTheorem(IplImage *image, struct Regions *regions);
IplImage *processNormalize(IplImage *image, struct Regions *regions);
IplImage *processNormalizeInc(IplImage *image, struct Regions *regions);
IplImage* processPhong(IplImage *image, int xLight, int yLight, int zLight, int Shine, int xObs, int yObs, int zObs, int numberRegions, map<int, struct Region> regions, bool enableSpecular, float ambiente, bool withBorder, IplImage *imageOriginal);
IplImage *processThreshold(IplImage *imageOriginal, IplImage *imagePhong, IplImage *imageColor, map<int, struct Region> &regions, int numberRegions, float percentil, int increase_pixel, bool withBorder, int xLight, int yLight,bool filled, bool lightness, int hatchingSize);
IplImage *processNormalizeImage(IplImage *image, struct Regions *regions);
IplImage *processNormalizeScalar(IplImage *image, struct Regions *regions,int scalar);
bool compareImageSize(char *filename1, char *filename2);
IplImage *GetImage(char *filename);

#endif