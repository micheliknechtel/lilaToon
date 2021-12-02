#include "stdafx.h"
#include "region.h"
#include "custom_open_cv.h"
#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <windows.h>
#include <omp.h>

//map<int, map<int, int>> flushfill(vector<int> vX, vector<int> vY)
//{
//	map<int, map<int, int>> flush;
//	for(int i = 0; i < vX.size(); i++)
//	{
//		flush[vX.at(i)][vY.at(i)] = i;
//	}
//}
//
//int getposflush(map<int, map<int, int>> &flush, x , y)
//{
//	if( flush.find(x) == flush.end())
//	{
//		return -1;
//	}
//
//	if(flush[x].find(y) == flush[x].end())
//	{
//		return -1;
//	}
//	
//	return flush[x][y];
//
//}

void filter(vector<float> *nX, vector<float> *nY)
{
	//Equation: ni = 1/16( n(i-2) + 4 n(i-1) + 6 n(i) + 4 n (i+1) + n(i+2))
	float x, y;
	vector<float> tempX;
	vector<float> tempY;

	if(nX != NULL && nX->size() != 0)
	{
		tempX.push_back(nX->at(0));
		tempY.push_back(nY->at(0));
		tempX.push_back(nX->at(1));
		tempY.push_back(nY->at(1));

		for(int i = 2; i <= nX->size() -3;i++)
		{
		  x = (nX->at(i-2) + 4 * nX->at(i-1) + 6 * nX->at(i) + 4 * nX->at(i+1) +  nX->at(i+2))/16;
		  y = (nY->at(i-2) + 4 * nY->at(i-1) + 6 * nY->at(i) + 4 * nY->at(i+1) +  nY->at(i+2))/16;
		  tempX.push_back(x);
		  tempY.push_back(y);
		}
		tempX.push_back(nX->at(nX->size()-2));
		tempY.push_back(nY->at(nY->size()-2));

		tempX.push_back(nX->at(nX->size()-1));
		tempY.push_back(nY->at(nY->size()-1));

		nX->clear();
		nX->assign(tempX.begin(),tempX.end());
		
		nY->clear();
		nY->assign(tempY.begin(),tempY.end());
	}
}

void normalPlaneXYArray(struct Region &region, map<int, struct Region> &regions, int numberRegions)
{
	//Equation:  nX(i) = (vY(i+1) - vY(i)) * -1;
	//Equation:  nY(i) = (vX(i+1) - vX(i));
	float norma;
	int x, y, xNext, yNext, X, Y;
	int iSize = 0;

	norma = 1;

	iSize = region.contour_arr_x.size();

	iSize += regions[region.id_father].contour_arr_x.size();

	for(vector<vector<int>>::size_type i = 0; i < region.branch_arr_x.size();i++)
		iSize += region.branch_arr_x[i].size();

	for(vector<vector<int>>::size_type i = 0; i < region.isolated_arr_x.size();i++)
		iSize += region.isolated_arr_x[i].size();

	region.normal.contour_norm_x.reserve(iSize);
	region.normal.contour_norm_y.reserve(iSize);
	region.normal.contour_arr_x.reserve(iSize);
	region.normal.contour_arr_y.reserve(iSize);

	for(vector<vector<int>>::size_type i = 0; i < region.branch_arr_x.size();i++)
	{
		int sizex = region.branch_arr_x[i].size();
		if(sizex > 0)
		{
			for(vector<int>::size_type j = 0; j < region.branch_arr_x[i].size()-1;j++)
			{
				x = region.branch_arr_x[i].at(j);
				y = region.branch_arr_y[i].at(j);

				xNext = region.branch_arr_x[i].at(j+1);
				yNext = region.branch_arr_y[i].at(j+1);

				X = (yNext - y) * 1;
				Y = (x-xNext) * 1;

				norma = sqrt(((float)X * X + Y * Y));
				region.normal.contour_norm_x.push_back(X/norma);
				region.normal.contour_norm_y.push_back(Y/norma);

				region.normal.contour_arr_x.push_back(x);
				region.normal.contour_arr_y.push_back(y);
			}

			if(region.branch_arr_x[i].size() -1 > 0)
			{

				x = region.branch_arr_x[i].at(region.branch_arr_x[i].size() -1);
				y = region.branch_arr_y[i].at(region.branch_arr_x[i].size() -1);

				xNext = region.branch_arr_x[i].at(0);
				yNext = region.branch_arr_y[i].at(0);

				X = (yNext - y) * 1;
				Y = (x-xNext) * 1;

				norma = sqrt(((float)X * X + Y * Y));

				region.normal.contour_norm_x.push_back(X/norma);
				region.normal.contour_norm_y.push_back(Y/norma);

				region.normal.contour_arr_x.push_back(x);
				region.normal.contour_arr_y.push_back(y);
			}
		}
	}

	for(vector<vector<int>>::size_type i = 0; i < region.isolated_arr_x.size();i++)
	{
		for(vector<int>::size_type j = 0; j < region.isolated_arr_x[i].size()-1;j++)
		{
			x = region.isolated_arr_x[i].at(j);
			y = region.isolated_arr_y[i].at(j);

			xNext = region.isolated_arr_x[i].at(j+1);
			yNext = region.isolated_arr_y[i].at(j+1);

			X = (yNext - y) * 1;
			Y = (x-xNext) * 1;

			norma = sqrt(((float)X * X + Y * Y));

			region.normal.contour_norm_x.push_back(X/norma);
			region.normal.contour_norm_y.push_back(Y/norma);

			region.normal.contour_arr_x.push_back(x);
			region.normal.contour_arr_y.push_back(y);
		}
		if(region.isolated_arr_x[i].size()-1 > 0)
		{
			x = region.isolated_arr_x[i].at(region.isolated_arr_x[i].size()-1);
			y = region.isolated_arr_y[i].at(region.isolated_arr_x[i].size()-1);

			xNext = region.isolated_arr_x[i].at(0);
			yNext = region.isolated_arr_y[i].at(0);

			X = (yNext - y) * 1;
			Y = (x-xNext) * 1;

			norma = sqrt(((float)X * X + Y * Y));

			region.normal.contour_norm_x.push_back(X/norma);
			region.normal.contour_norm_y.push_back(Y/norma);

			region.normal.contour_arr_x.push_back(x);
			region.normal.contour_arr_y.push_back(y);
		}
	}

	if(region.contour_arr_x.size() != 0)
	{
		for(vector<int>::size_type i = 0; i < region.contour_arr_x.size()-1;i++)
		{
			x = region.contour_arr_x[i];
			y = region.contour_arr_y[i];

			xNext = region.contour_arr_x[i+1];
			yNext = region.contour_arr_y[i+1];

			X = (yNext - y) * 1;
			Y = (x-xNext) * 1;

			norma = sqrt(((float)X * X + Y * Y));

			region.normal.contour_norm_x.push_back(X/norma);
			region.normal.contour_norm_y.push_back(Y/norma);
			region.normal.contour_arr_x.push_back(x);
			region.normal.contour_arr_y.push_back(y);
		}
		if(region.contour_arr_x.size()-1 > 0)
		{
			x = region.contour_arr_x[region.contour_arr_x.size()-1];
			y = region.contour_arr_y[region.contour_arr_x.size()-1];

			xNext = region.contour_arr_x[0];
			yNext = region.contour_arr_y[0];

			X = (yNext - y) * 1;
			Y = (x-xNext) * 1;

			norma = sqrt(((float)X * X + Y * Y));

			region.normal.contour_norm_x.push_back(X/norma);
			region.normal.contour_norm_y.push_back(Y/norma);
			region.normal.contour_arr_x.push_back(x);
			region.normal.contour_arr_y.push_back(y);
		}

		if(region.id_father != 0 && region.change == false)
		{
			if(regions[region.id_father].contour_arr_x.size() != 0)
			{
				for(vector<int>::size_type i = 0; i < regions[region.id_father].contour_arr_x.size()-1;i++)
				{

					x = regions[region.id_father].contour_arr_x[i];
					y = regions[region.id_father].contour_arr_y[i];

					xNext = regions[region.id_father].contour_arr_x[i+1];
					yNext = regions[region.id_father].contour_arr_y[i+1];

					X = (yNext - y) * 1;
					Y = (x-xNext) * 1;

					norma = sqrt(((float)X * X + Y * Y));

					region.normal.contour_norm_x.push_back(X/norma);
					region.normal.contour_norm_y.push_back(Y/norma);
					region.normal.contour_arr_x.push_back(x);
					region.normal.contour_arr_y.push_back(y);
				}

				if(regions[region.id_father].contour_arr_x.size()-1 > 0)
				{
					x = regions[region.id_father].contour_arr_x[regions[region.id_father].contour_arr_x.size()-1];
					y = regions[region.id_father].contour_arr_y[regions[region.id_father].contour_arr_x.size()-1];

					xNext = regions[region.id_father].contour_arr_x[0];
					yNext = regions[region.id_father].contour_arr_y[0];

					X = (yNext - y) * 1;
					Y = (x-xNext) * 1;

					norma = sqrt(((float)X * X + Y * Y));

					region.normal.contour_norm_x.push_back(X/norma);
					region.normal.contour_norm_y.push_back(Y/norma);
					region.normal.contour_arr_x.push_back(x);
					region.normal.contour_arr_y.push_back(y);
				}
			}
		}
	}
}

void normalPlaneXYArrayInc(vector<int> vX, vector<int> vY, vector<float> &fX, vector<float> &fY, struct Region &region)
{
	//Equation:  nX(i) = (vY(i+1) - vY(i)) * -1;
	//Equation:  nY(i) = (vX(i+1) - vX(i));
	float norma;
	int x, y, xNext, yNext, X, Y;
	int iSize = 0;

	norma = 1;

	if(vX.size() != 0)
	{
		for(vector<int>::size_type i = 0; i < vX.size()-1;i++)
		{
			x = vX[i];
			y = vY[i];

			xNext = vX[i+1];
			yNext = vY[i+1];

			X = (yNext - y) * 1;
			Y = (x-xNext) * 1;

			norma = sqrt(((float)X * X + Y * Y));

			/*if(region.id == 0)
			{
				region.normal.fill_arr_x.push_back(x);
				region.normal.fill_arr_y.push_back(y);
			}*/

			if(norma == 0)
			{
				fX.push_back(0);
				fY.push_back(0);
			}
			else
			{
				region.normal.contour_norm_x.push_back(X/norma);
				region.normal.contour_norm_y.push_back(Y/norma);
				region.normal.contour_arr_x.push_back(x);
				region.normal.contour_arr_y.push_back(y);
				fX.push_back(X/norma);
				fY.push_back(Y/norma);
			}

		}
		if(vX.size()-1 > 0)
		{
			x = vX[vX.size()-1];
			y = vY[vX.size()-1];

			xNext = vX[0];
			yNext = vY[0];

			X = (yNext - y) * 1;
			Y = (x-xNext) * 1;

			norma = sqrt(((float)X * X + Y * Y));

			//if(region.id == 0)
			//{
			//	region.normal.fill_arr_x.push_back(x);
			//	region.normal.fill_arr_y.push_back(y);
			//}

			if(norma != 0)
			{
				region.normal.contour_norm_x.push_back(X/norma);
				region.normal.contour_norm_y.push_back(Y/norma);
				region.normal.contour_arr_x.push_back(x);
				region.normal.contour_arr_y.push_back(y);
				fX.push_back(X/norma);
				fY.push_back(Y/norma);
			}
			else
			{
				fX.push_back(0);
				fY.push_back(0);
			}
		}
	}
}


float normalZ(float npX, float npY)
{
	// Equation : npZ = npX^2 + npY^2 - 1;

    float npZ = 0;

	npZ = sqrt(1 - npX * npX - npY * npY);
	//npZ = 1 - npX - npY ;
	return npZ;
}

float normalZscalar(float npX, float npY, int scalar)
{
	// Equation : npZ = npX^2 + npY^2 - 1;
    float norma;
    float npZ = 0;

	/*npX = npX * scalar;
	npY = npY * scalar;*/
	//norma = sqrt(((float)npX * npX + npY * npY + scalar * scalar));
	//npX = (npX* scalar)/norma;
	//npY = (npY* scalar)/norma;
	npZ = sqrt(1 - npX * npX - npY * npY);
	//norma = sqrt(npZ * npZ + scalar * scalar);
	//npZ = npZ/norma;


	//npZ = 1 - npX - npY ;
	return npZ;
}

void normalXY(const int pX, const int pY, int *array_x, int *array_y, float *normal_x, float *normal_y, int size_array, float *_wp, float *_sumX, float *_sumY, bool is_invert)
{
	// Equation : npx = 1/w(pk) * sum (nxi/((pk - vi)^2)) where i start in 0 until i = n;

	register float wp = 0;
	register float sumX = 0;
	register float sumY = 0;
	register float partial = 0;
	int a, b, x, y, i;
	int invert = 1;

	if(is_invert)
		invert = -1;

	for (i = 0; i < size_array; i++)
    {
		//partial = sqrt((vX->at(i)-pX)*(vX->at(i)-pX)+(vY->at(i)-pY)*(vY->at(i)-pY));
		//partial = pow((float)vX->at(i)-pX,2)+ pow((float)vY->at(i)-pY,2);
		//partial = pow(vX->at(i)-pX,2)+ pow(vY->at(i)-pY,2);
		x = array_x[i];
		y = array_y[i];

		a = x-pX;
		b = y-pY;
		
		partial = (a * a + b * b);

		if( partial == 0)
			continue;
		
		sumX += ((normal_x[i] * invert)/partial);
		sumY += ((normal_y[i] * invert)/partial);
		
		//// Equation : w(pk) = sum (1/(pk - vi)^2) where i start in 0 until i = n;
		wp	+= (1/partial);
	}
	//wp = wp;

	//*_wp = wp;
	*_wp = wp ;
	*_sumX = sumX;
	*_sumY = sumY;
	/*
	sumX = wp * sumX ;
	sumY = wp * sumY ;

	*X = wp * sumX ;
	*Y = wp * sumY;
	*Z = normalZ(*Z, *Y);*/
}


//inline void normalXY(const int pX, const int pY, int *array_x, int *array_y, float *normal_x, float *normal_y, int size_array, float *X, float *Y, float *Z, bool is_invert)
//{
//	// Equation : npx = 1/w(pk) * sum (nxi/((pk - vi)^2)) where i start in 0 until i = n;
//
//	register float wp = 0;
//	register float sumX = 0;
//	register float sumY = 0;
//	register float partial = 0;
//	int a, b, x, y;
//	int invert = 1;
//
//	if(is_invert)
//		invert = -1;
//
//	for (register int i = 0; i < size_array; i++)
//    {
//		//partial = sqrt((vX->at(i)-pX)*(vX->at(i)-pX)+(vY->at(i)-pY)*(vY->at(i)-pY));
//		//partial = pow((float)vX->at(i)-pX,2)+ pow((float)vY->at(i)-pY,2);
//		//partial = pow(vX->at(i)-pX,2)+ pow(vY->at(i)-pY,2);
//		x = array_x[i];
//		y = array_y[i];
//
//		a = x-pX;
//		b = y-pY;
//		
//		partial = (a * a + b * b);
//
//		if(partial == 0)
//			continue;
//		
//		sumX += ((normal_x[i] * invert)/partial);
//		sumY += ((normal_y[i] * invert)/partial);
//		
//		//// Equation : w(pk) = sum (1/(pk - vi)^2) where i start in 0 until i = n;
//		wp	+= (1/partial);
//	}
//
//	sumX = wp * sumX ;
//	sumY = wp * sumY ;
//
//	*X = sumX;
//	*Y = sumY;
//	*Z = normalZ(sumX, sumY);
//}

void normalize(map<int, struct Region> &regions, int numberRegions)
{
	int x, y;
	float X, Y, Z;
	float wp, sumX, sumY;
	int iSize = 0;
	int *array_x;
	int *array_y;
	float *normal_x;
	float *normal_y;
	float norma;
	int size_array;

	normalPlaneXYArray(regions[0], regions, numberRegions);
//	filter(&regions[0].normal.contour_norm_x, &regions[0].normal.contour_norm_y);
	size_array = regions[0].normal.contour_arr_x.size();
	array_x = new int[size_array];
	array_y = new int[size_array];
	normal_x = new float[size_array];
	normal_y = new float[size_array];
	std::copy(regions[0].normal.contour_arr_x.begin(), regions[0].normal.contour_arr_x.end(),array_x);
	std::copy(regions[0].normal.contour_arr_y.begin(), regions[0].normal.contour_arr_y.end(),array_y);
	std::copy(regions[0].normal.contour_norm_x.begin(), regions[0].normal.contour_norm_x.end(),normal_x);
	std::copy(regions[0].normal.contour_norm_y.begin(), regions[0].normal.contour_norm_y.end(),normal_y);

	for(vector<vector<int>>::size_type i = 0; i < regions[0].branch_arr_x.size();i++)
		iSize += regions[0].branch_arr_x[i].size();

	regions[0].normal.fill_norm_x.reserve(iSize);
	regions[0].normal.fill_norm_y.reserve(iSize);
	regions[0].normal.fill_norm_z.reserve(iSize);
	regions[0].normal.fill_arr_x.reserve(iSize);
	regions[0].normal.fill_arr_y.reserve(iSize);

	for(vector<vector<int>>::size_type i = 0; i < regions[0].isolated_arr_x.size();i++)
	{
		for(vector<int>::size_type j = 0; j < regions[0].isolated_arr_x[i].size();j++)
		{
			x = regions[0].isolated_arr_x[i].at(j);
			y = regions[0].isolated_arr_y[i].at(j);

			//normalXY(x, y, regions[0], &wp, &sumX, &sumY);
			wp = 0; sumX = 0; sumY = 0;
			normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, true);
			X = (1/wp) * sumX;
			Y = (1/wp) * sumY;
			Z = normalZ(X,Y);
			regions[0].normal.wp.push_back(wp);
			regions[0].normal.sumX.push_back(sumX);
			regions[0].normal.sumY.push_back(sumY);
			regions[0].normal.fill_norm_x.push_back(X);
			regions[0].normal.fill_norm_y.push_back(Y);
			regions[0].normal.fill_norm_z.push_back(Z);
			regions[0].normal.fill_arr_x.push_back(x);
			regions[0].normal.fill_arr_y.push_back(y);
		}
	}
	free(array_x);
	free(array_y);
	free(normal_x);
	free(normal_y);

	for(int r = 1; r < numberRegions;r++)
	{
		bool invert_contour = false;
		bool invert_isolated = true;

		iSize = 0;
		normalPlaneXYArray(regions[r], regions, numberRegions);
//		filter(&regions[r].normal.contour_norm_x, &regions[r].normal.contour_norm_y);
		size_array = regions[r].normal.contour_arr_x.size();

		iSize = 0;

		for(vector<vector<int>>::size_type i = 0; i < regions[r].branch_arr_x.size();i++)
			iSize += regions[r].branch_arr_x[i].size();

		for(vector<vector<int>>::size_type i = 0; i < regions[r].isolated_arr_x.size();i++)
			iSize += regions[r].isolated_arr_x[i].size();

		iSize += regions[r].contour_arr_x.size();
		iSize += regions[r].fill_arr_x.size();

		regions[r].normal.fill_norm_x.reserve(iSize);
		regions[r].normal.fill_norm_y.reserve(iSize);
		regions[r].normal.fill_norm_z.reserve(iSize);
		regions[r].normal.fill_arr_x.reserve(iSize);
		regions[r].normal.fill_arr_y.reserve(iSize);

		array_x = new int[size_array];
		array_y = new int[size_array];
		normal_x = new float[size_array];
		normal_y = new float[size_array];
		std::copy(regions[r].normal.contour_arr_x.begin(), regions[r].normal.contour_arr_x.end(),array_x);
		std::copy(regions[r].normal.contour_arr_y.begin(), regions[r].normal.contour_arr_y.end(),array_y);
		std::copy(regions[r].normal.contour_norm_x.begin(), regions[r].normal.contour_norm_x.end(),normal_x);
		std::copy(regions[r].normal.contour_norm_y.begin(), regions[r].normal.contour_norm_y.end(),normal_y);

		for(vector<vector<int>>::size_type i = 0; i < regions[r].branch_arr_x.size();i++)
		{
			for(vector<int>::size_type j = 0; j < regions[r].branch_arr_x.at(i).size();j++)
			{
				x = regions[r].branch_arr_x.at(i).at(j);
				y = regions[r].branch_arr_y.at(i).at(j);
				//normalXY(x, y, regions[r], &wp, &sumX, &sumY);
				wp = 0; sumX = 0; sumY = 0;
				normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_isolated);
				X = (1/wp) * sumX;
				Y = (1/wp) * sumY;
				Z = normalZ(X,Y);
				regions[r].normal.wp.push_back(wp);
				regions[r].normal.sumX.push_back(sumX);
				regions[r].normal.sumY.push_back(sumY);

				regions[r].normal.fill_norm_x.push_back(X);
				regions[r].normal.fill_norm_y.push_back(Y);
				regions[r].normal.fill_norm_z.push_back(Z);
				regions[r].normal.fill_arr_x.push_back(x);
				regions[r].normal.fill_arr_y.push_back(y);
			}
		}

		for(vector<vector<int>>::size_type i = 0; i < regions[r].isolated_arr_x.size();i++)
		{
			for(vector<int>::size_type j = 0; j < regions[r].isolated_arr_x[i].size();j++)
			{
				x = regions[r].isolated_arr_x.at(i).at(j);
				y = regions[r].isolated_arr_y.at(i).at(j);
				//normalXY(x, y, regions[r], &wp, &sumX, &sumY);
				wp = 0; sumX = 0; sumY = 0;
				normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_isolated);
				X = (1/wp) * sumX;
				Y = (1/wp) * sumY;
				Z = normalZ(X,Y);
				regions[r].normal.wp.push_back(wp);
				regions[r].normal.sumX.push_back(sumX);
				regions[r].normal.sumY.push_back(sumY);

				regions[r].normal.fill_norm_x.push_back(X);
				regions[r].normal.fill_norm_y.push_back(Y);
				regions[r].normal.fill_norm_z.push_back(Z);
				regions[r].normal.fill_arr_x.push_back(x);
				regions[r].normal.fill_arr_y.push_back(y);
			}
		}

		for(vector<int>::size_type i = 0; i < regions[r].fill_arr_x.size();i++)
		{
			x = regions[r].fill_arr_x[i];
			y = regions[r].fill_arr_y[i];
			//normalXY(x, y, regions[r], &wp, &sumX, &sumY);
			wp = 0; sumX = 0; sumY = 0;
			normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_contour);
			X = (1/wp) * sumX;
			Y = (1/wp) * sumY;
			Z = normalZ(X,Y);
			regions[r].normal.wp.push_back(wp);
			regions[r].normal.sumX.push_back(sumX);
			regions[r].normal.sumY.push_back(sumY);

			regions[r].normal.fill_norm_x.push_back(X);
			regions[r].normal.fill_norm_y.push_back(Y);
			regions[r].normal.fill_norm_z.push_back(Z);
			regions[r].normal.fill_arr_x.push_back(x);
			regions[r].normal.fill_arr_y.push_back(y);
		}

		/*if(r == 6)
			dilate(&regions[r].contour_arr_x, &regions[r].contour_arr_y, SQUARE_3X3, 9);*/

		for(vector<int>::size_type i = 0; i < regions[r].contour_arr_x.size();i++)
		{
			x = regions[r].contour_arr_x[i];
			y = regions[r].contour_arr_y[i];
			//normalXY(x, y, regions[r], &wp, &sumX, &sumY);
			wp = 0; sumX = 0; sumY = 0;
			normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_contour);
			X = (1/wp) * sumX;
			Y = (1/wp) * sumY;
			Z = normalZ(X,Y);
			regions[r].normal.wp.push_back(wp);
			regions[r].normal.sumX.push_back(sumX);
			regions[r].normal.sumY.push_back(sumY);

			regions[r].normal.fill_norm_x.push_back(X);
			regions[r].normal.fill_norm_y.push_back(Y);
			regions[r].normal.fill_norm_z.push_back(Z);
			regions[r].normal.fill_arr_x.push_back(x);
			regions[r].normal.fill_arr_y.push_back(y);
		}
		free(array_x);
		free(array_y);
		free(normal_x);
		free(normal_y);
	}
}

void normalize_plus(struct Region &region, map<int, struct Region> &regions, int numberRegions)
{
	int x, y;
	float X, Y, Z;
	float wp, sumX, sumY;
	int iSize = 0;
	int *array_x;
	int *array_y;
	float *normal_x;
	float *normal_y;
	int size_array;

	bool invert_contour = false;
	bool invert_isolated = true;

	iSize = 0;
	normalPlaneXYArray(region, regions, numberRegions);
//		filter(&region.normal.contour_norm_x, &region.normal.contour_norm_y);
	size_array = region.normal.contour_arr_x.size();

	iSize = 0;

	for(vector<vector<int>>::size_type i = 0; i < region.branch_arr_x.size();i++)
		iSize += region.branch_arr_x[i].size();

	for(vector<vector<int>>::size_type i = 0; i < region.isolated_arr_x.size();i++)
		iSize += region.isolated_arr_x[i].size();

	iSize += region.contour_arr_x.size();
	iSize += region.fill_arr_x.size();

	region.normal.fill_norm_x.reserve(iSize);
	region.normal.fill_norm_y.reserve(iSize);
	region.normal.fill_norm_z.reserve(iSize);
	region.normal.fill_arr_x.reserve(iSize);
	region.normal.fill_arr_y.reserve(iSize);

	array_x = new int[size_array];
	array_y = new int[size_array];
	normal_x = new float[size_array];
	normal_y = new float[size_array];
	std::copy(region.normal.contour_arr_x.begin(), region.normal.contour_arr_x.end(),array_x);
	std::copy(region.normal.contour_arr_y.begin(), region.normal.contour_arr_y.end(),array_y);
	std::copy(region.normal.contour_norm_x.begin(), region.normal.contour_norm_x.end(),normal_x);
	std::copy(region.normal.contour_norm_y.begin(), region.normal.contour_norm_y.end(),normal_y);

	for(vector<vector<int>>::size_type i = 0; i < region.branch_arr_x.size();i++)
	{
		for(vector<int>::size_type j = 0; j < region.branch_arr_x.at(i).size();j++)
		{
			x = region.branch_arr_x.at(i).at(j);
			y = region.branch_arr_y.at(i).at(j);
			//normalXY(x, y, region.id, &wp, &sumX, &sumY);
			wp = 0; sumX = 0; sumY = 0;
			normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_isolated);
			X = (1/wp) * sumX;
			Y = (1/wp) * sumY;
			Z = normalZ(X,Y);
			region.normal.wp.push_back(wp);
			region.normal.sumX.push_back(sumX);
			region.normal.sumY.push_back(sumY);

			region.normal.fill_norm_x.push_back(X);
			region.normal.fill_norm_y.push_back(Y);
			region.normal.fill_norm_z.push_back(Z);
			region.normal.fill_arr_x.push_back(x);
			region.normal.fill_arr_y.push_back(y);
		}
	}

	for(vector<vector<int>>::size_type i = 0; i < region.isolated_arr_x.size();i++)
	{
		for(vector<int>::size_type j = 0; j < region.isolated_arr_x[i].size();j++)
		{
			x = region.isolated_arr_x.at(i).at(j);
			y = region.isolated_arr_y.at(i).at(j);
			//normalXY(x, y, region.id, &wp, &sumX, &sumY);
			wp = 0; sumX = 0; sumY = 0;
			normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_isolated);
			X = (1/wp) * sumX;
			Y = (1/wp) * sumY;
			Z = normalZ(X,Y);
			region.normal.wp.push_back(wp);
			region.normal.sumX.push_back(sumX);
			region.normal.sumY.push_back(sumY);

			region.normal.fill_norm_x.push_back(X);
			region.normal.fill_norm_y.push_back(Y);
			region.normal.fill_norm_z.push_back(Z);
			region.normal.fill_arr_x.push_back(x);
			region.normal.fill_arr_y.push_back(y);
		}
	}

	for(vector<int>::size_type i = 0; i < region.fill_arr_x.size();i++)
	{
		x = region.fill_arr_x[i];
		y = region.fill_arr_y[i];
		//normalXY(x, y, region.id, &wp, &sumX, &sumY);
		wp = 0; sumX = 0; sumY = 0;
		normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_contour);
		X = (1/wp) * sumX;
		Y = (1/wp) * sumY;
		Z = normalZ(X,Y);
		region.normal.wp.push_back(wp);
		region.normal.sumX.push_back(sumX);
		region.normal.sumY.push_back(sumY);

		region.normal.fill_norm_x.push_back(X);
		region.normal.fill_norm_y.push_back(Y);
		region.normal.fill_norm_z.push_back(Z);
		region.normal.fill_arr_x.push_back(x);
		region.normal.fill_arr_y.push_back(y);
	}

	/*if(r == 6)
		dilate(&region.contour_arr_x, &region.contour_arr_y, SQUARE_3X3, 9);*/

	for(vector<int>::size_type i = 0; i < region.contour_arr_x.size();i++)
	{
		x = region.contour_arr_x[i];
		y = region.contour_arr_y[i];
		//normalXY(x, y, region.id, &wp, &sumX, &sumY);
		wp = 0; sumX = 0; sumY = 0;
		normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, invert_contour);
		X = (1/wp) * sumX;
		Y = (1/wp) * sumY;
		Z = normalZ(X,Y);
		region.normal.wp.push_back(wp);
		region.normal.sumX.push_back(sumX);
		region.normal.sumY.push_back(sumY);

		region.normal.fill_norm_x.push_back(X);
		region.normal.fill_norm_y.push_back(Y);
		region.normal.fill_norm_z.push_back(Z);
		region.normal.fill_arr_x.push_back(x);
		region.normal.fill_arr_y.push_back(y);
	}
	free(array_x);
	free(array_y);
	free(normal_x);
	free(normal_y);
}

void normalize_inc(struct Region &region, vector<int> vX, vector<int> vY)
{
	vector<float> fX, fY;
	int x, y;
	float X, Y, Z;
	float wp, sumX, sumY;
	int iSize = 0;
	int *array_x;
	int *array_y;
	int *fill_x;
	int *fill_y;
	float *normal_x;
	float *normal_y;
	int size_array, size_array_fill;
	bool invert_contour = false;
	bool invert_isolated = true;
	register int i;

	if(region.id == 0)
	{
		region.isolated_arr_x.push_back(vX);
		region.isolated_arr_y.push_back(vY);

		for(int n = 0;n < vX.size(); n++)
		{
			region.normal.fill_arr_x.push_back(vX[n]);
			region.normal.fill_arr_y.push_back(vY[n]);
			region.normal.fill_norm_x.push_back(0);
			region.normal.fill_norm_y.push_back(0);
			region.normal.fill_norm_z.push_back(0);
			region.normal.wp.push_back(0);
			region.normal.sumX.push_back(0);
			region.normal.sumY.push_back(0);

			region.normal.contour_arr_x.push_back(vX[n]);
			region.normal.contour_arr_y.push_back(vY[n]);

		}
	}

	iSize = 0;
	normalPlaneXYArrayInc(vX, vY, fX, fY, region);
	size_array = vX.size();

	array_x = new int[size_array];
	array_y = new int[size_array];
	normal_x = new float[size_array];
	normal_y = new float[size_array];
	std::copy(vX.begin(), vX.end(),array_x);
	std::copy(vY.begin(), vY.end(),array_y);
	std::copy(fX.begin(), fX.end(),normal_x);
	std::copy(fY.begin(), fY.end(),normal_y);

	size_array_fill = region.normal.fill_arr_x.size();
	fill_x = new int[size_array_fill];
	fill_y = new int[size_array_fill];
	std::copy(region.normal.fill_arr_x.begin(), region.normal.fill_arr_x.end(),fill_x);
	std::copy(region.normal.fill_arr_y.begin(), region.normal.fill_arr_y.end(),fill_y);

	#pragma omp parallel for shared(fill_x, fill_y, region) private(x, y, wp, sumX, sumY, X, Y, Z, i)	
	for(i = 0; i < size_array_fill;i++)
	{
		
		x = fill_x[i];
		y = fill_y[i];
		//fprintf(fp, "i = %d - thread = %d\n",i, omp_get_thread_num());
		normalXY(x, y, array_x, array_y, normal_x, normal_y, size_array, &wp, &sumX, &sumY, false);
		wp = (region.normal.wp[i]+wp);
		sumX = region.normal.sumX[i] + sumX;
		sumY = region.normal.sumY[i] + sumY;
		if(wp == 0)
			continue;
		X = (1/wp) * sumX;
		Y = (1/wp) * sumY;
		Z = normalZ(X,Y);
		region.normal.wp[i] = wp;
		region.normal.sumX[i] = sumX;
		region.normal.sumY[i] = sumY;
		region.normal.fill_norm_x[i] = X;
		region.normal.fill_norm_y[i] = Y;
		region.normal.fill_norm_z[i] = Z;
	}
	free(array_x);
	free(array_y);
	free(normal_x);
	free(normal_y);
}

