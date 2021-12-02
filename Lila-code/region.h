#ifndef __REGION__
#define __REGION__

#include "stdafx.h"

#include <vector>
#include <map>

using namespace std;

struct RegionNormal
{
	vector<int> fill_arr_x;
	vector<int> fill_arr_y;
	vector<float> fill_arr_z;
	vector<int> contour_arr_x;
	vector<int> contour_arr_y;
	vector<float> contour_norm_x;
	vector<float> contour_norm_y;
	vector<float> fill_norm_x;
	vector<float> fill_norm_y;
	vector<float> fill_norm_z;
	vector<float> wp;
	vector<float> sumX;
	vector<float> sumY;
};

struct Region
{	
	int		id;
	int		id_father;
	int		color;
	int     paint_color;
	bool	change;
	vector<int> fill_arr_x;
	vector<int> fill_arr_y;
	vector<int> contour_arr_x;
	vector<int> contour_arr_y;
	vector<vector<int>> branch_arr_x;
	vector<vector<int>> branch_arr_y;
	vector<vector<int>> isolated_arr_x;
	vector<vector<int>> isolated_arr_y;
	int		start_point_x;
	int		start_point_y;
	struct RegionNormal normal;
	bool direction;
};

struct Regions
{
	int size;
	map<int, struct Region> regions;
};

extern map<int, map<int, vector<int>>> mapindex;

#endif
