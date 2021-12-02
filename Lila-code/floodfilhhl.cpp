#include "stdafx.h"
#include "custom_open_cv.h"
#include "stack.h"
#include "region.h"
#include "floodfill.h"

#include <algorithm>

int REGION_COUNT = 0;
int COLOR_SIZE = 0;
int ARRAY_COLOR[REGION_SIZE];

int getRandomColor()
{
	bool exists = true;
	int random = ((rand() % PIXEL_LIMIT)+1) * ((rand() % PIXEL_LIMIT)+1) * ((rand() % PIXEL_LIMIT)+1);

	while(exists != false && COLOR_SIZE < REGION_SIZE )
	{
		exists = false;
		for(int i = 0; i < COLOR_SIZE; i++)
		{
			if(ARRAY_COLOR[i] == random)
			{
				exists = true;
			}
		}

		random = ((rand() % PIXEL_LIMIT)+1) * ((rand() % PIXEL_LIMIT)+1) * ((rand() % PIXEL_LIMIT)+1);
	}

	if(COLOR_SIZE < REGION_SIZE)
	{
		ARRAY_COLOR[COLOR_SIZE++] = random; 
	}
	else
	{
		random = 0;
	}

	return random;
}

void floodFill4Stack(int x, int y, int oldColor, IplImage *image, vector<int> &array_x, vector<int> &array_y, int &start_x, int &start_y, int reg_color)
{
	int w = image->width;
	int h = image->height;
	bool ChainCode = true;
	int color;
	int newColor = reg_color;
	int pos = 0;
	int old_start_x = x;
	int old_start_y = y;

	if(newColor == oldColor) return ; //avoid infinite loop
		emptyStack();
	

	if(!push(x, y,h)) return ; 

	while(pop(x, y,h))
	{

		int p2 ;
		GET_PIXEL4INT(image,x,y-1,&p2);

		int p4 ;
		GET_PIXEL4INT(image,x+1,y,&p4);

		int p6 ;		
		GET_PIXEL4INT(image,x,y+1,&p6);

		int p8 ;
		GET_PIXEL4INT(image,x-1,y,&p8);		

		int p3;
		GET_PIXEL4INT(image,x+1,y-1,&p3);

		int p5;
		GET_PIXEL4INT(image,x+1,y+1,&p5);

		int p7;		
		GET_PIXEL4INT(image,x-1,y+1,&p7);

		int p9;
		GET_PIXEL4INT(image,x-1,y-1,&p9);

		// Get current color
		GET_PIXEL4INT(image,x,y,&color);

		if(color != newColor)
		{
			SET_PIXEL4INT(image,x,y,newColor);
			array_x.push_back(x);
			array_y.push_back(y);
		}	

		//if(ChainCode && x + 1 < w && p4 == 0)
		//{			
		//	start_x = x+1;
		//	start_y = y;
		//	ChainCode = false;
		//}

		//if(ChainCode && x - 1 > 0 && p8 == 0)
		//{
		//	start_x = x-1;
		//	start_y = y;
		//	ChainCode = false;
		//}
		if(ChainCode && y + 1 < h-1 && p6 == 0 && p5 != 0)
		{
			start_x = x;
			start_y = y+1;
			ChainCode = false;
		}
		if(ChainCode && y - 1 > 0 && p2 == 0 && p3 != 0)
		{
			start_x = x;
			start_y = y-1;
			ChainCode = false;
		}

		if(ChainCode && y + 1 < h-1 && p7 == 0 && p6 != 0)
		{
			start_x = x-1;
			start_y = y+1;
			ChainCode = false;
		}
		if(ChainCode && y - 1 > 0 && p9 == 0 && p2 != 0)
		{
			start_x = x-1;
			start_y = y-1;
			ChainCode = false;
		}

		if(ChainCode && y + 1 < h-1 && p6 == 0)
		{
			start_x = x;
			start_y = y+1;
		}
		if(ChainCode && y - 1 > 0 && p2 == 0)
		{
			start_x = x;
			start_y = y-1;
		}

		if(ChainCode && y + 1 < h-1 && p7 == 0)
		{
			start_x = x-1;
			start_y = y+1;
		}
		if(ChainCode && y - 1 > 0 && p9 == 0)
		{
			start_x = x-1;
			start_y = y-1;
		}

		if(x + 1 < w-1 && p4 == oldColor)
		{   
			if(!push(x + 1, y, h))
			{	
				break;           
			}
		}    
		if(x - 1 > 0 && p8 == oldColor)
		{
			if(!push(x - 1, y, h))
			{	
				break;           
			}
		}    
		if(y + 1 < h-1 && p6 == oldColor)
		{
			if(!push(x, y + 1, h))
			{
				break;
			}
		}    
		if(y - 1 > 0 && p2 == oldColor)
		{
			if(!push(x, y - 1, h))
			{
				break;  
			}         
		}    
	}
}

int doFloodFill(IplImage *image, map<int, struct Region> &regions)
{
	int p;
	bool flag = true;
	REGION_COUNT = 0;
	COLOR_SIZE = 0;
	int start_x, start_y;
	regions.clear();

	for (int y = 1; y < image->height - 1; y++)
	//for (int y = image->height - 2; y >= 1; y--)
	{
		for (int x = 1; x < image->width - 1; x++)
		//for (int x = image->width - 2; x >= 1; x--)
		{
			GET_PIXEL4INT(image, x, y, &p);
			if(p == WHITE_COLOR)
			{ 
				int random = getRandomColor();
				regions[REGION_COUNT].id = REGION_COUNT;
				regions[REGION_COUNT].id_father = 0;
				regions[REGION_COUNT].color = random;
				floodFill4Stack(x, y, RGB2Int(255, 255, 255), image, regions[REGION_COUNT].fill_arr_x, regions[REGION_COUNT].fill_arr_y , start_x, start_y, regions[REGION_COUNT].color);
				regions[REGION_COUNT].start_point_x = start_x;
				regions[REGION_COUNT].start_point_y = start_y;
				REGION_COUNT++;
			}
		}
	}
	return REGION_COUNT;
}
