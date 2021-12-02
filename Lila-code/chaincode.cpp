#include "stdafx.h"
#include "custom_open_cv.h"
#include "region.h"
#include "stdio.h"
#include <algorithm>

int region_id;
struct _point
{
	int x;
	int y;
};

struct _point arr_points[8] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

#define P2 0
#define P3 1
#define P4 2
#define P5 3
#define P6 4
#define P7 5
#define P8 6
#define P9 7
#define UX 1
#define DX 2
#define UY 3
#define DY 4

//int ORDER_CHAIN [8] = {P6, P7, P8, P9, P2, P3, P4, P5};
int ORDER_CHAIN [8] = {P2, P9, P8, P7, P6, P5, P4, P3};
//int ORDER_CHAIN [8] = {P4, P5, P6, P7, P8, P9, P2, P3};
//int ORDER_CHAIN [8] = {P4, P3, P2, P9, P8, P7, P6, P5};
//int ORDER_CHAIN [8] = {P9, P8, P7, P6, P5, P4, P3, P2};
//int ORDER_CHAIN [8] = {P2, P3, P4, P5, P6, P7, P8, P9};
//int ORDER_CHAIN [8] = {P7, P8, P9, P2, P3, P4, P5, P6};

void doChainCodeImage(IplImage *image, int start_x, int start_y, vector<int> &array_x, vector<int> &array_y, vector<int> &branch_start_x, vector<int> &branch_start_y, int reg_color);
void doBranchChainCodeImage(IplImage *image,int x, int y, vector<int> &array_x, vector<int> &array_y, int color);
void refill(IplImage *image, int numberRegions, map<int, struct Region> &regions);

inline bool isIncorrectDirection(vector<int> *vX, vector<int> *vY)
{
	int uX = 0, uY = 0, dX, dY;
	int found = 0;
	int seq_identify[2];

	seq_identify[0] = 0;
	seq_identify[1] = 0;

	if(vX != NULL && vX->size() != 0)
	{
		dX = vX->at(0);
		dY = vY->at(0);
		for(int i = 0; i < vX->size(); i++)
		{
			if(uX < vX->at(i))
				uX = vX->at(i);

			if(uY < vY->at(i))
				uY = vY->at(i);

			if(dX > vX->at(i))
				dX = vX->at(i);

			if(dY > vY->at(i))
				dY = vY->at(i);
		}

		for(int i = 0; i < vX->size() && found != 2; i++)
		{
			if(uX == vX->at(i) && seq_identify[0] != UX)
			{
				seq_identify[found] = UX;
				found++;
			}
			if(dX == vX->at(i) && seq_identify[0] != DX)
			{
				seq_identify[found] = DX;
				found++;
			}
			if(uY == vY->at(i) && seq_identify[0] != UY)
			{
				seq_identify[found] = UY;
				found++;
			}
			if(dY == vY->at(i) && seq_identify[0] != DY)
			{
				seq_identify[found] = DY;
				found++;
			}
		}

		if ((
			(seq_identify[0] == DX && seq_identify[1] == UY) ||
			(seq_identify[0] == UY && seq_identify[1] == UX) ||	
			(seq_identify[0] == UX && seq_identify[1] == DY) ||	
			(seq_identify[0] == DY && seq_identify[1] == DX) ))
		{
			return true;
		}

	}
	return false;
}

void InvertDirection(vector<int> *vX, vector<int> *vY)
{
	vector<int> tempX;
	vector<int> tempY;

	for(int i = vX->size()-1;i >= 0; i--)
	{
		tempX.push_back(vX->at(i));
		tempY.push_back(vY->at(i));
	}

	vX->clear();
	vY->clear();

	vX->assign(tempX.begin(), tempX.end());
	vY->assign(tempY.begin(), tempY.end());
}

inline int countNeiborhoodColor(IplImage *image, int x, int y, int color)
{
	int p;
	int count = 0;

    for(register int a = 0;a < 8;a+=2)
	{
		if(x+arr_points[ORDER_CHAIN[a]].x > 0 && y+arr_points[ORDER_CHAIN[a]].y > 0 && x+arr_points[ORDER_CHAIN[a]].x < image->width && y+arr_points[ORDER_CHAIN[a]].y < image->height)
		{
			GET_PIXEL4INT(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);

			if(p == color)
				count++;
		}
	}
	return count;
}

inline bool isBranchPoint(IplImage *image, int x, int y, int color, bool onlyBranch)
{
	int result = 1;
	int p;

    for(register int a = 0;a < 8;a++)
	{
		GET_PIXEL4INT(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);
		result *= ((p == BLACK_COLOR) || (p == color) || (p == WHITE_COLOR) || onlyBranch) ? 1 : 0;
	}

	return result == 1;
}

inline int isNeiborhoodPoint(IplImage *image,int x, int y,int color)
{	
	int result = 0;
	int p;

    for(register int a = 0;a < 8;a++)
	{
		GET_PIXEL4INT(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);
		if(p == color)
			result++;
	}

	if(result != 0)
	{
		result = color;
	}

	return result;
}

inline int isNeiborhood(IplImage *image,int x, int y,int color, vector<int> &array_x, vector<int> &array_y, int doBranchNextPoint )
{	
	int lastX    = array_x.size()> 3 ? array_x[array_x.size()-2]:0;
	int lastY    = array_y.size()> 3 ? array_y[array_y.size()-2]:0;

	int lastAuxX = array_x.size() > 3?array_x[array_x.size()-1]:0;
	int lastAuxY = array_y.size() > 3?array_y[array_y.size()-1]:0;

	int result = 0;

	if(doBranchNextPoint == 0)
	{
		//p2
		if((x == lastX) && (y == lastY - 1) )
		{ 
			return 0;
		}

		//p4
		if((x == lastX + 1) && (y == lastY ) )
		{ 
			return 0;
		}

		//p6
		if((x == lastX) && (y == lastY + 1) )
		{ 
			return 0;
		}

		//p8
		if((x == lastX-1) && (y == lastY) )
		{ 
			return 0;
		}
	}

	return isNeiborhoodPoint(image, x, y,color);
}

inline bool isSplittedPoint(IplImage *image, int x, int y, int color, bool onlyBranch)
{
	int P[8];
	int result = 0;
	int p;

    for(register int a = 0;a < 8;a++)
	{
		GET_PIXEL4INT(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);
		P[a] = (p == BLACK_COLOR && isNeiborhoodPoint(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,color) != 0) ? 1 : 0;
		result += P[a];
	}

	return result >= 2;
}

inline bool searchBranchPoint(IplImage *image, int x, int y, vector<int> &array_x, vector<int> &array_y, int color, int doNextPoint, bool onlyBranch)
{
	int p;
	int auxX = x;
	int auxY = y;

	doNextPoint = 0;

	for(register int a = 0;a < 8;a++)
	{
		GET_PIXEL4INT(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);

		if((p == 0) && (isBranchPoint(image, x+arr_points[ORDER_CHAIN[a]].x, y+arr_points[ORDER_CHAIN[a]].y, color, onlyBranch)))
		{
			auxX = x + arr_points[ORDER_CHAIN[a]].x;
			auxY = y + arr_points[ORDER_CHAIN[a]].y;

			if(onlyBranch)
				doBranchChainCodeImage(image, auxX, auxY, array_x, array_y, color);
			else
			{
				array_x.push_back(x);
				array_y.push_back(y);
//				mapindex[x][y].push_back(region_id);
			}

			doNextPoint = 1;
		}
	}

	if(doNextPoint == 1)
	{
		return true;
	}

	return false;
}

inline bool searchNewSplittedPoint(IplImage *image, int x, int y, vector<int> &array_x, vector<int> &array_y, int color, int doNextPoint, bool onlyBranch)
{
	int auxX = x;
	int auxY = y;

	int p ;

	doNextPoint = 0;

	for(register int a = 0;a < 8;a++)
	{
		GET_PIXEL4INT(image,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);

		if(p == 0 && isSplittedPoint(image, x+arr_points[ORDER_CHAIN[a]].x, y+arr_points[ORDER_CHAIN[a]].y, color, onlyBranch))
		{
			auxX = x + arr_points[ORDER_CHAIN[a]].x;
			auxY = y + arr_points[ORDER_CHAIN[a]].y;
			
			if(searchBranchPoint(image, auxX, auxY, array_x, array_y, color, 0,onlyBranch))
				doNextPoint = 1;
		}
	}
	
	if(doNextPoint == 1)
	{
		return true;
	}
	return false;
}


inline bool isDirection(IplImage *image, int x, int y, int color, bool onlyBranch, vector<int> &array_x, vector<int> &array_y, int doBranchNextPoint)
{
	int p ;	
	bool doNextPoint = false;
	GET_PIXEL4INT(image,x,y,&p);
	if(p == BLACK_COLOR && (isBranchPoint(image,x,y,color, onlyBranch) == onlyBranch) )
	{
		if(isNeiborhood(image, x, y, color, array_x, array_y, doBranchNextPoint) == color)
		{
			array_x.push_back(x);
			array_y.push_back(y);
//			mapindex[x][y].push_back(region_id);
			SET_PIXEL4INT(image, x, y, WHITE_COLOR);
			doNextPoint = true;
		}
	}
	return doNextPoint;
}

inline void chaincode(IplImage *image, int &x, int &y, vector<int> &array_x, vector<int> &array_y, int color, int &doNextPoint,bool onlyBranch)
{
	doNextPoint = 0;
    for(register int a = 0;a < 8;a++)
	{
		if(doNextPoint == 0 && isDirection(image, x + arr_points[ORDER_CHAIN[a]].x, y + arr_points[ORDER_CHAIN[a]].y, color, onlyBranch, array_x, array_y, doNextPoint))
		{
			doNextPoint = 1;
			x = x + arr_points[ORDER_CHAIN[a]].x;
			y = y + arr_points[ORDER_CHAIN[a]].y;
		}
	}
}

void doBranchChainCodeImage(IplImage *image,int x, int y, vector<int> &array_x, vector<int> &array_y, int color)
{
	int p;

	GET_PIXEL4INT(image, x, y, &p);
	if(p == BLACK_COLOR)
	{
		array_x.push_back(x);
		array_y.push_back(y);
//		mapindex[x][y].push_back(region_id);
		SET_PIXEL4INT(image,x,y,WHITE_COLOR);
	}

	int START_X = x;
	int START_Y = y;

	int doNextPoint = 1;

	do
	{
		doNextPoint = 0;

		if(isSplittedPoint(image, x, y, color,true))
		{
			if( ! searchBranchPoint(image, x, y, array_x, array_y, color, doNextPoint, true))
			{
				searchNewSplittedPoint(image, x, y, array_x, array_y, color, doNextPoint,true);
			}
		}

		chaincode(image, x, y, array_x, array_y, color, doNextPoint, true);
		
	} while((START_X != x || START_Y != y) && doNextPoint == 1);
}


void doChainCodeImage(IplImage *image, int start_x, int start_y,  vector<int> &array_x, vector<int> &array_y, vector<int> &branch_start_x, vector<int> &branch_start_y, int reg_color)
{ 
	int p, x, y, color;

	x = start_x;
	y = start_y;
	color = reg_color;

	GET_PIXEL4INT(image, x, y, &p);
	
	if(p == BLACK_COLOR)
	{
		array_x.push_back(x);
		array_y.push_back(y);
//		mapindex[x][y].push_back(region_id);
		SET_PIXEL4INT(image,x,y,WHITE_COLOR);
	}

	int START_X = x;
	int START_Y = y;

	int doNextPoint = 1;

	do
	{
		doNextPoint = 0;

		if(isSplittedPoint(image, x, y, color,false))
		{
			if( ! searchBranchPoint(image,x, y, branch_start_x, branch_start_y, color, doNextPoint,false))
			{
				searchNewSplittedPoint(image, x, y, branch_start_x, branch_start_y, color, doNextPoint,false);
			}
		}

		chaincode(image, x, y, array_x, array_y, color, doNextPoint, false);
		
	} while((START_X != x || START_Y != y) && doNextPoint == 1);
}

int getRegionPosition(IplImage *image, int x, int y, map<int, struct Region> &regions, int numberRegions)
{
    register int p;
    bool found = false;
    for(p = 0; p < numberRegions; p++)
    {
        if(isNeiborhoodPoint(image, x, y, regions[p].color) != 0)
        {
            found = true;
            break;
        }
    }
    
    if(found)
        return p;

    return -1;    
}

inline void selectIsolatedPoints(IplImage *image, int numberRegions, map<int, struct Region> &regions)
{
	/* Prepara a imagem */
	for (int r = 1; r < regions.size();r++)
	{
		for(int p = 0; p < regions[r].contour_arr_x.size();p++)
		{
			SET_PIXEL4INT(image, regions[r].contour_arr_x[p], regions[r].contour_arr_y[p], WHITE_COLOR);
		}
	}
	
	for (int r = 1; r < regions.size();r++)
	{
		for(int b = 0; b < regions[r].branch_arr_x.size();b++)
		{
			for(int p = 0; p < regions[r].branch_arr_x[b].size();p++)
			{
				SET_PIXEL4INT(image, regions[r].branch_arr_x[b].at(p), regions[r].branch_arr_y[b].at(p), WHITE_COLOR);
			}
		}
	}

	for(register int y = 1; y < image->height -1; y++)
    {
        for(register int x = 1; x < image->width -1; x++)
        {
		    int p, r;

			GET_PIXEL4INT(image, x, y, &p);
			if(p == BLACK_COLOR)
            {
				r = getRegionPosition(image, x, y, regions, numberRegions);
				if( r != -1)
				{
					region_id = r;
					vector<int> isolated_x = vector<int>::vector();
					vector<int> isolated_y = vector<int>::vector();

					doBranchChainCodeImage(image, x, y, isolated_x, isolated_y,regions[r].color);

					if(regions[r].direction)
						InvertDirection(&isolated_x, &isolated_y);

					regions[r].isolated_arr_x.push_back(isolated_x);
					regions[r].isolated_arr_y.push_back(isolated_y);
					//mapindex[isolated_x][isolated_y].push_back(region_id);
				}
            }    
        }
    }
}

IplImage *ChainCode(IplImage *image,
					int numberRegions, 
					map<int, struct Region> &regions)
{
	IplImage *imageShow;
	vector<int> branch_start_x;
	vector<int> branch_start_y;

	for(int a = 0; a < numberRegions;a++)
	{
		region_id = a;
		imageShow = cvCloneImage(image);
		doChainCodeImage(imageShow, regions[a].start_point_x, regions[a].start_point_y, regions[a].contour_arr_x, regions[a].contour_arr_y, branch_start_x, branch_start_y, regions[a].color);
		bool dir = isIncorrectDirection(&regions[a].contour_arr_x, &regions[a].contour_arr_y);

		if(regions[a].change)
			dir = !dir;
		
		regions[a].direction = dir;
		
		if(dir)
			InvertDirection(&regions[a].contour_arr_x, &regions[a].contour_arr_y);

		for(vector<int>::size_type b = 0; b < branch_start_x.size();b++)
		{
			vector<int> branch_x = vector<int>::vector();
			vector<int> branch_y = vector<int>::vector();
			doBranchChainCodeImage(imageShow, branch_start_x[b], branch_start_y[b], branch_x, branch_y, regions[a].color);
			if(dir)
				InvertDirection(&branch_x, &branch_y);
			
			regions[a].branch_arr_x.push_back(branch_x);
			regions[a].branch_arr_y.push_back(branch_y);
			//mapindex[branch_x][branch_y].push_back(region_id);
		}


		branch_start_x.clear();
		branch_start_y.clear();
		
		// Limpa ramos que não estão com vector nulos (que foram previamente visitados)
		cvReleaseImage(&imageShow);
	}

	IplImage *imageClear = cvCloneImage(image);

	selectIsolatedPoints(imageClear, numberRegions, regions);

	refill(imageClear, numberRegions, regions);

	return imageClear;
}

void refill(IplImage *image,
			int numberRegions,
		    map<int, struct Region> &regions)
{
	IplImage *imageTF;
	int p;
	char name[1024];

	for (int i = 1; i < numberRegions;i++)
	{
		region_id = i;
		imageTF = createImageRGB(image->width, image->height);
		cvRectangle(imageTF, cvPoint(0,0), cvPoint(image->width, image->height), cvScalar(255,255,255),-1);

		for(vector<int>::size_type j = 0; j < regions[i].fill_arr_x.size(); j++)
		{
			SET_PIXEL4INT(imageTF, regions[i].fill_arr_x.at(j), regions[i].fill_arr_y.at(j), regions[i].color);
		}

		for(vector<int>::size_type j = 0; j < regions[i].contour_arr_x.size(); j++)
		{
			SET_PIXEL4INT(imageTF, regions[i].contour_arr_x.at(j), regions[i].contour_arr_y.at(j), regions[i].color);
		}

		for(vector<vector<int>>::size_type b = 0; b < regions[i].branch_arr_x.size(); b++)
		{
			for(vector<int>::size_type j = 0; j < regions[i].branch_arr_x.at(b).size();j++)
			{
				SET_PIXEL4INT(imageTF, regions[i].branch_arr_x.at(b).at(j), regions[i].branch_arr_y.at(b).at(j), regions[i].color);
			}
		}

		for(vector<vector<int>>::size_type b = 0; b < regions[i].isolated_arr_x.size(); b++)
		{
			for(vector<int>::size_type j = 0; j < regions[i].isolated_arr_x.at(b).size();j++)
			{
				SET_PIXEL4INT(imageTF, regions[i].isolated_arr_x.at(b).at(j), regions[i].isolated_arr_y.at(b).at(j), regions[i].color);
			}
		}

		for(vector<int>::size_type j = 0; j < regions[i].fill_arr_x.size(); j++)
		{
			int x = regions[i].fill_arr_x.at(j);
			int y = regions[i].fill_arr_y.at(j);

			for(int a = 0;a < 8;a++)
			{

				GET_PIXEL4INT(imageTF,x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y,&p);

				if(p == WHITE_COLOR && countNeiborhoodColor(imageTF, x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y, regions[i].color) > 3)
				{
					SET_PIXEL4INT(imageTF, x+arr_points[ORDER_CHAIN[a]].x,y+arr_points[ORDER_CHAIN[a]].y, regions[i].color);
					regions[i].fill_arr_x.push_back(x+arr_points[ORDER_CHAIN[a]].x);
					regions[i].fill_arr_y.push_back(y+arr_points[ORDER_CHAIN[a]].y);
//					mapindex[x+arr_points[ORDER_CHAIN[a]].x][y+arr_points[ORDER_CHAIN[a]].y].push_back(region_id);
				}
			}
		}

		//sprintf(name,"region_%02d.png", i);
		//cvSaveImage(name,imageTF);
		cvReleaseImage(&imageTF);
	}
}
