#include "stdafx.h"
#include "process.h"
#include "custom_open_cv.h"
#include <stdio.h>
#include <math.h>

IplImage *GetImage(char *filename)
{
	IplImage *img;

	if( (img = cvLoadImage (filename,-1)) == 0) 
		return NULL;

	return img;
}

bool compareImageSize(char *filename1, char *filename2)
{
	IplImage *img1;
	IplImage *img2;
	int h1, h2, w1, w2;

	if( (img1 = cvLoadImage (filename1,-1)) == 0) 
		return false;

	h1 = img1->height;
	w1 = img1->width;

	if( (img2 = cvLoadImage (filename2,-1)) == 0) 
		return false;

	h2 = img2->height;
	w2 = img2->width;

	cvReleaseImage(&img1);
	cvReleaseImage(&img2);

	if(h1 != h2 || w1 != w2)
		return false;


	return true;
}

IplImage *processThinning(char *filename_original)
{
	IplImage *imageOriginal, *imageOutput;

	if( (imageOriginal = cvLoadImage (filename_original,-1)) == 0) 
		return NULL;
	IplImage *imageThinning = thinning(imageOriginal);

	imageOutput = createImageRGB(imageThinning->width, imageThinning->height);
	cvCvtColor(imageThinning, imageOutput, CV_GRAY2RGB);
	cvReleaseImage(&imageThinning);

	return imageOutput;
}

IplImage *processFloodFill(IplImage *image, struct Regions *regions)
{
	IplImage *imageFloodFill = cvCloneImage(image);
	
	int numberRegions = doFloodFill(imageFloodFill, regions->regions);
	regions->size = numberRegions;
	return imageFloodFill;
}

IplImage *processChaincode(IplImage *image, struct Regions *regions)
{
	IplImage *imageChaincode;

	imageChaincode = ChainCode(image, regions->size, regions->regions);

	return imageChaincode;
}

IplImage *processColor(char *filename_color)
{
	IplImage *imageColor;

	if( (imageColor = cvLoadImage (filename_color,-1)) == 0) 
		return NULL;

	return imageColor;
}

IplImage *processWhite(IplImage *image)
{
	IplImage *imageWhite = cvCloneImage(image);
	cvRectangle(imageWhite,cvPoint(0,0),cvPoint(imageWhite->width,imageWhite->height),cvScalar(255,255,255),-1);

	return imageWhite;
}

IplImage *processMakeColor(char *filename_color,struct Regions *regions)
{
	IplImage *imageColor;

	if( (imageColor = cvLoadImage (filename_color,-1)) == 0) 
		return NULL;


	IplImage *imageMakecolor = MakeColorImage(imageColor, regions->size, regions->regions);

	cvReleaseImage(&imageColor);

	return imageMakecolor;
}

void processJordanTheorem(IplImage *image, struct Regions *regions)
{
	treeRegions(regions->size, regions->regions, image->height, image->width);
}

IplImage *processNormalize(IplImage *image, struct Regions *regions)
{
	normalize(regions->regions, regions->size);

	IplImage *imageNormal = createImageRGB(image->width,image->height);
	cvRectangle(imageNormal, cvPoint(0,0), cvPoint(image->width,image->height),cvScalar(255,255,255),-1);

	for(int r = 0; r < regions->size;r++)
	{
		for(vector<int>::size_type i = 0; i < regions->regions[r].normal.fill_arr_x.size();i++)
		{
			int x = regions->regions[r].normal.fill_arr_x.at(i);
			int y = regions->regions[r].normal.fill_arr_y.at(i);
			SET_PIXELRGB(imageNormal, x, y, ((regions->regions[r].normal.fill_norm_z[i]+1)/2)*255,0);
			SET_PIXELRGB(imageNormal, x, y, ((regions->regions[r].normal.fill_norm_x[i]+1)/2)*255,1);
			SET_PIXELRGB(imageNormal, x, y, ((regions->regions[r].normal.fill_norm_y[i]+1)/2)*255,2);
		}
	}
	return imageNormal;
}

IplImage *processNormalizeInc(IplImage *image, struct Regions *regions)
{
	IplImage *imageNormal = createImageRGB(image->width,image->height);
	cvRectangle(imageNormal, cvPoint(0,0), cvPoint(image->width,image->height),cvScalar(255,255,255),-1);

	for(int r = 0; r < regions->size;r++)
	{
		for(vector<int>::size_type i = 0; i < regions->regions[r].normal.fill_arr_x.size();i++)
		{
			int x = regions->regions[r].normal.fill_arr_x.at(i);
			int y = regions->regions[r].normal.fill_arr_y.at(i);
			SET_PIXELRGB(imageNormal, x, y, ((regions->regions[r].normal.fill_norm_z[i]+1)/2)*255,0);
			SET_PIXELRGB(imageNormal, x, y, ((regions->regions[r].normal.fill_norm_x[i]+1)/2)*255,1);
			SET_PIXELRGB(imageNormal, x, y, ((regions->regions[r].normal.fill_norm_y[i]+1)/2)*255,2);
		}
	}
	return imageNormal;
}

IplImage *processNormalizeScalar(IplImage *image, struct Regions *regions,int scalar)
{
	IplImage *imageNormal = createImageRGB(image->width,image->height);
	cvRectangle(imageNormal, cvPoint(0,0), cvPoint(image->width,image->height),cvScalar(255,255,255),-1);

	float value;

	for(int r = 0; r < regions->size;r++)
	{
		for(vector<int>::size_type i = 0; i < regions->regions[r].normal.fill_arr_x.size();i++)
		{
			int x = regions->regions[r].normal.fill_arr_x.at(i);
			int y = regions->regions[r].normal.fill_arr_y.at(i);
			float wp = regions->regions[r].normal.wp.at(i);
			float sumX = regions->regions[r].normal.sumX.at(i);
			float sumY = regions->regions[r].normal.sumY.at(i);
			float X = (1/wp) * sumX;
			float Y = (1/wp) * sumY;
			float Z = normalZscalar(X,Y, scalar);
			float norma = scalar * 1.1;//sqrt(X*scalar*X*scalar+Y*scalar*Y*scalar+Z*scalar*Z*scalar);
			
			if(scalar > 1)
			{
				Z = (Z * scalar/norma);
			}

			regions->regions[r].normal.fill_norm_x[i] = X;
			regions->regions[r].normal.fill_norm_y[i] = Y;
			regions->regions[r].normal.fill_norm_z[i] = Z;

			SET_PIXELRGB(imageNormal, x, y, ((Z+1)/2)*255,0);
			SET_PIXELRGB(imageNormal, x, y, ((X+1)/2)*255,1);
			SET_PIXELRGB(imageNormal, x, y, ((Y+1)/2)*255,2);
		}
	}
	return imageNormal;
}

IplImage *processNormalizeImage(IplImage *image, struct Regions *regions)
{

	IplImage *imageNormal = createImageRGB(image->width,image->height);
	cvRectangle(imageNormal, cvPoint(0,0), cvPoint(image->width,image->height),cvScalar(255,255,255),-1);

	for(int r = 0; r < regions->size;r++)
	{
		int iSize = regions->regions[r].normal.contour_arr_x.size();
		for(vector<int>::size_type i = 0; i < iSize -1; i++)
		{
			int x = regions->regions[r].normal.contour_arr_x.at(i);
			int y = regions->regions[r].normal.contour_arr_y.at(i);
			int xNext = regions->regions[r].normal.contour_arr_x.at(i+1);
			int yNext = regions->regions[r].normal.contour_arr_y.at(i+1);
			float distX = regions->regions[r].normal.contour_norm_x.at(i);
			float distY = regions->regions[r].normal.contour_norm_y.at(i);
			
			int X = x + distX * 5;
			int Y = y + distY * 5;

			cvLine(imageNormal, cvPoint(x, y), cvPoint(X, Y), cvScalar(0,0,255), 1);

			SET_PIXELRGB(imageNormal,x, y, 255, 0);
			SET_PIXELRGB(imageNormal,x, y, 0, 1);
			SET_PIXELRGB(imageNormal,x, y, 0, 2);
		}
	}
	return imageNormal;
}

IplImage* processPhong(IplImage *image, int xLight, int yLight, int zLight, int Shine, int xObs, int yObs, int zObs, int numberRegions, map<int, struct Region> regions, bool enableSpecular, float ambiente, bool withBorder, IplImage *imageOriginal)
{
	IplImage *imagePhong = cvCloneImage(image);

	point light , centerCanvas;
	light.x = xLight;
	light.y = yLight;
	light.z = zLight;
	
	centerCanvas.x = xObs;
	centerCanvas.y = yObs;
	centerCanvas.z = zObs;
	for (int r = 0; r < numberRegions;r++)
	{
		int R, G, B;
		int x, y;
		float Ipr, Ipg, Ipb;

		for(vector<int>::size_type i = 0; i < regions[r].normal.fill_arr_x.size(); i++)
		{
			x = regions[r].normal.fill_arr_x[i];
			y = regions[r].normal.fill_arr_y[i];
			Int2RGB(regions[r].paint_color,&B,&G,&R);
			//GET_PIXELRGB(image,x, y, 0, &R);
			//GET_PIXELRGB(image,x, y, 1, &G);
			//GET_PIXELRGB(image,x, y, 2, &B);
			Phong(R, G, B, regions[r].normal.fill_norm_x[i], regions[r].normal.fill_norm_y[i], regions[r].normal.fill_norm_z[i], x, y, Shine, light, centerCanvas, &Ipr, &Ipg, &Ipb, image->width, image->height, enableSpecular, ambiente);
			SET_PIXELRGB(imagePhong, x, y, Ipr,0);
			SET_PIXELRGB(imagePhong, x, y, Ipg,1);
			SET_PIXELRGB(imagePhong, x, y, Ipb,2);
		}
	}

	if(withBorder)
	{
		int width = imageOriginal->width;
		int height = imageOriginal->height;
		IplImage *imageOriginal2 = createImage(width, height, 8, 1);

		cvCvtColor(imageOriginal, imageOriginal2, CV_BGR2GRAY);

		cvThreshold(imageOriginal2, imageOriginal2, 100, 255, CV_THRESH_BINARY);

		for(int k = 0; k < imageOriginal2->width;k++)
		{
			for(int l = 0; l < imageOriginal2->height;l++)
			{
				int p0;
				GET_PIXEL(imageOriginal2,k,l,&p0);
				
				if(p0 == 0)
				{
					SET_PIXELRGB(imagePhong,k,l,p0,0);
					SET_PIXELRGB(imagePhong,k,l,p0,1);
					SET_PIXELRGB(imagePhong,k,l,p0,2);
				}
			}
		}
		cvReleaseImage(&imageOriginal2);
	}

	return imagePhong;
}

IplImage *processThreshold(IplImage *imageOriginal, IplImage *imagePhong, IplImage *imageColor, map<int, struct Region> &regions, int numberRegions, float percentil, int increase_pixel, bool withBorder, int xLight, int yLight,bool filled, bool lightness, int hatchingSize)
{
	IplImage *imageTemp = processWhite(imagePhong);
	IplImage *imageThreshold = thresholdImage(imagePhong, imageTemp, regions, numberRegions, percentil, increase_pixel, true);
	IplImage *imageClone = cvCloneImage(imageColor);
	IplImage *imageHatching;
	int p0, p1, p2;

	for(int r = 1; r < numberRegions;r++)
	{
	   imageHatching = processWhite(imagePhong);
	   Int2RGB(regions[r].paint_color, &p0, &p1, &p2);
	   cvRectangle(imageHatching,cvPoint(0,0),cvPoint(imagePhong->width,imagePhong->height),CV_RGB(p0,p1,p2),-1);


	   for(int k = 0;k < regions[r].contour_arr_x.size();k+=hatchingSize)
	   {
		   int i = regions[r].contour_arr_x[k];
		   int j = regions[r].contour_arr_y[k];

		   Int2RGB(regions[r].paint_color, &p0, &p1, &p2);

		   p0 -= increase_pixel;
		   p1 -= increase_pixel;
		   p2 -= increase_pixel;
		   p0 = p0 < 0 ? 0: p0;
		   p1 = p1 < 0 ? 0: p1;
		   p2 = p2 < 0 ? 0: p2;

		   cvLine(imageHatching,cvPoint(xLight,yLight),cvPoint(i,j),CV_RGB(p0,p1,p2),1,CV_AA);
		   cvLine(imageHatching,cvPoint(imagePhong->width-xLight,imagePhong->height-yLight),cvPoint(i,j),CV_RGB(p0,p1,p2),1, CV_AA);
	   }

	   for(int k = 0;k < regions[r].fill_arr_x.size();k++)
	   {
		   int i = regions[r].fill_arr_x[k];
		   int j = regions[r].fill_arr_y[k];
		   int phong;

		   GET_PIXELRGB(imageHatching,i,j,0,&p0);
		   GET_PIXELRGB(imageHatching,i,j,1,&p1);
		   GET_PIXELRGB(imageHatching,i,j,2,&p2);
		   if(p0 != 255 || p1 != 255 || p2 != 255)
		   {
				if(lightness)
				{
					GET_PIXELRGB(imagePhong,i,j,0,&phong);	
					p0 = (p0 * phong)/255;
					p1 = (p1 * phong)/255;
					p2 = (p2 * phong)/255;
				}
				

			   SET_PIXELRGB(imageTemp,i,j,p0,0);
			   SET_PIXELRGB(imageTemp,i,j,p1,1);
			   SET_PIXELRGB(imageTemp,i,j,p2,2);
		   }
	   }
	   cvReleaseImage(&imageHatching);
	}

	for(int i = 0; i<imagePhong->width;i++)
	{
	   for(int j = 0; j<imagePhong->height;j++)
	   {
		   int t;
		   GET_PIXELRGB(imageThreshold,i,j,0,&t);
		   if(t > 200)
		   {
			   SET_PIXELRGB(imageTemp,i,j,255,0);
			   SET_PIXELRGB(imageTemp,i,j,255,1);
			   SET_PIXELRGB(imageTemp,i,j,255,2);
		   }
	   }
	}

	for(int r = 1; r < numberRegions;r++)
	   for(int k = 0;k < regions[r].contour_arr_x.size();k++)
	   {

			   int i = regions[r].contour_arr_x[k];
			   int j = regions[r].contour_arr_y[k];

			   Int2RGB(regions[r].paint_color, &p2, &p1, &p0);

			   p0 -= increase_pixel;
			   p1 -= increase_pixel;
			   p2 -= increase_pixel;
			   p0 = p0 < 0 ? 0: p0;
			   p1 = p1 < 0 ? 0: p1;
			   p2 = p2 < 0 ? 0: p2;

			   SET_PIXELRGB(imageTemp,i,j,p0,0);
			   SET_PIXELRGB(imageTemp,i,j,p1,1);
			   SET_PIXELRGB(imageTemp,i,j,p2,2);

	   }

	if(withBorder)
	{
		int width = imageOriginal->width;
		int height = imageOriginal->height;
		IplImage *imageOriginal2 = createImage(width, height, 8, 1);

		cvCvtColor(imageOriginal, imageOriginal2, CV_BGR2GRAY);

		cvThreshold(imageOriginal2, imageOriginal2, 100, 255, CV_THRESH_BINARY);

		for(int k = 0; k < imageOriginal2->width;k++)
		{
			for(int l = 0; l < imageOriginal2->height;l++)
			{
				GET_PIXEL(imageOriginal2,k,l,&p0);
				
				if(p0 == 0)
				{
					SET_PIXELRGB(imageTemp,k,l,p0,0);
					SET_PIXELRGB(imageTemp,k,l,p0,1);
					SET_PIXELRGB(imageTemp,k,l,p0,2);
				}
			}
		}
		cvReleaseImage(&imageOriginal2);
	}

	if(filled)
	{
		int width = imageTemp->width;
		int height = imageTemp->height;
		IplImage *imageAux = createImage(width, height, 8, 1);

		//cvCvtColor(imageTemp, imageAux, CV_BGR2GRAY);

		//cvThreshold(imageAux, imageAux, 200, 255, CV_THRESH_BINARY);

		for(int k = 0; k < imageAux->width;k++)
		{
			for(int l = 0; l < imageAux->height;l++)
			{
				GET_PIXELRGB(imageTemp,k,l,0,&p0);
				GET_PIXELRGB(imageTemp,k,l,1,&p1);
				GET_PIXELRGB(imageTemp,k,l,2,&p2);

				if(p0 == 255 && p1 == 255 && p2 == 255)
				{
					GET_PIXELRGB(imageClone,k,l,0,&p0);
					GET_PIXELRGB(imageClone,k,l,1,&p1);
					GET_PIXELRGB(imageClone,k,l,2,&p2);
					
					SET_PIXELRGB(imageTemp,k,l,p0,0);
					SET_PIXELRGB(imageTemp,k,l,p1,1);
					SET_PIXELRGB(imageTemp,k,l,p2,2);
				}
			}
		}
	}

	return imageTemp;
}

