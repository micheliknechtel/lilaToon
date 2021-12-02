#include "stdafx.h"
#include "region.h"
#include "custom_open_cv.h"

bool beContained(int start_x, int start_y, vector<int> &array_x, vector<int> &array_y, int h, int w)
{
	register int x, y, lastX, result, auxX;
	register int resultinv, lastXinv;

	result = 0;
	resultinv = 0;
	lastX = 0;
	lastXinv = 0;
	auxX = 0;

	for(vector<int>::size_type n = 0; n < array_x.size(); n++)
	{
		x = array_x.at(n);
		y = array_y.at(n);
		if(start_y == y)
		{
			if(start_x <= x && w >= x)
			{
				if(x != lastX+1 && y != lastX-1)
				{
					result++;
				}
				lastX = x;
			}

			if(0 <= x && start_x >= x)
			{
				if(x != lastXinv+1 && x != lastXinv-1)
				{
					resultinv++;
				}

				if(n != 0)
					lastXinv = auxX;
			}
			auxX = x;
		}
	}

	return (( (result%2) == 1) && ( (resultinv%2) == 1));
}

void getMiddlePoint(vector<int> &array_x, vector<int> &array_y, int &x, int &y, int h, int w)
{
	register int _x, _y;
	register int uX = w, uY = h, lX = w, lY = y, rX = 0, rY = 0, dX = 0, dY = 0;
	for(vector<int>::size_type i = 0; i < array_x.size();i++)
	{
		_x = array_x.at(i);
		_y = array_y.at(i);

		if(uY > _y)
		{
			uX = _x;
			uY = _y;
		}
		if(dY < _y)
		{
			dX = _x;
			dY = _y;
		}
		if(lX > _x)
		{
			lX = _x;
			lY = _y;
		}
		if(rX < _x)
		{
			rX = _x;
			rY = _y;
		}
	}
	x = ((int)(uX+lX+rX+dX)/4);
	y = ((int)(uY+lY+rY+dY)/4);

	while( ! beContained(x, y, array_x, array_y, h, w) && (x < rX  || y < dY))
	{
		if(x < rX)
		{
			x++;
		}	
		if(y < dY)
		{
			y++;
		}	
	}
}

void treeRegions(int numberRegions, map<int, struct Region> &regions, int h, int w)
{
	int old, x , y;
	/* Define inicialmente que todas as regiões sao filhas da regiao 0 */

	for(register int r = 1; r < numberRegions; r++)
	{
		for(register int z = 1; z < numberRegions; z++)
		{
			if(r != z)
			{
				if(beContained(regions[r].start_point_x, regions[r].start_point_y, regions[z].contour_arr_x, regions[z].contour_arr_y, h, w ))
				{
					getMiddlePoint(regions[r].contour_arr_x, regions[r].contour_arr_y, x, y, h, w);
					if(beContained(x, y, regions[z].contour_arr_x, regions[z].contour_arr_y,h, w ))
					{
						if( regions[r].id_father != 0)
						{
							old = regions[r].id_father;
							
							if(!(beContained(regions[old].start_point_x, regions[old].start_point_y, regions[z].contour_arr_x, regions[z].contour_arr_y, h, w)))
								regions[r].id_father = z;
							else
							{
								getMiddlePoint(regions[old].contour_arr_x, regions[old].contour_arr_y, x, y, h, w);
								if(!(beContained(x, y, regions[z].contour_arr_x, regions[z].contour_arr_y, h, w)))
									regions[r].id_father = z;
							}
						}
						else
						{
							regions[r].id_father = z;
						}
					}
				}
			}
		}
	}
}

