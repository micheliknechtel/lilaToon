#include "stdafx.h"
#include <stdio.h>
#include "region.h"
#include "histogram.h"
#include <map>
#include "custom_open_cv.h"

using namespace std;

struct Histogram *calculate_histogram_from_region(IplImage *image, vector<int> &array_x, vector<int> &array_y)
{
	int p, count;
	typedef map<int,int> MapTypeHist;
	MapTypeHist mapColor;
	struct Histogram *hist;
	vector<int> array_color;
	vector<int> array_count;

	int min_color, max_color, count_min, count_max;
	
	hist = (struct Histogram *)malloc(sizeof(struct Histogram));

	if(hist == NULL)
	{
		return NULL;
	}
	hist->size = 0;
	count = 0;

	min_color = -1;
	count_min = WHITE_COLOR;
	max_color = -1;
	count_max = 0;
	

	if(array_x.size() == 0)
	{
		hist->max_color = 0;
		hist->min_color = 0;
		hist->count = 0;
		hist->percentil_pixel = 0;
		hist->upper_color = 0;
		hist->lower_color = 0;
		return hist;
	}

	for(vector<int>::size_type i = 0; i < array_x.size();i++)
	{
		GET_PIXEL4INT(image, array_x[i], array_y[i], &p);
		mapColor[p]++;
		count++;
	}

	hist->size = mapColor.size();

	int i = 0;
	bool first = true;

	for(MapTypeHist::const_iterator it = mapColor.begin(); it != mapColor.end(); it++, i++)
	{
		int key = it->first;
		int value = it->second;
		if(first)
		{
			hist->lower_color = key;
			first = false;
		}
		hist->upper_color = key;
		
		array_color.push_back(key);
		array_count.push_back(value);
	}
	
	for( int i = 0; i < hist->size; i++)
	{
		if(count_min >= array_count[i] && array_count[i] != 0)
		{
			count_min = array_count.at(i);
			min_color = array_color.at(i);
		}
		if(count_max < array_count[i] && array_count[i] != 0)
		{
			count_max = array_count.at(i);
			max_color = array_color.at(i);
		}
	}
	mapColor.clear();
	hist->max_color = max_color;
	hist->min_color = min_color;
	hist->count = count;
	hist->percentil_pixel = 0;

	return hist;
}

struct Histogram *calculate_histogram_threshold(IplImage *image, vector<int> &array_x, vector<int> &array_y, float percentil)
{
	int p, count;
	typedef map<int,int> MapTypeHist;
	MapTypeHist mapColor;
	struct Histogram *hist;
	int min_color, max_color, count_min, count_max;
	int percentil_limit;
	int partial_count = 0;
	vector<int> array_color;
	vector<int> array_count;

	hist = (struct Histogram *)malloc(sizeof(struct Histogram));

	if(hist == NULL)
	{
		printf("Não foi possível alocar memória\n");
		exit(1);
	}
	hist->size = 0;
	count = 0;

	min_color = -1;
	count_min = WHITE_COLOR;
	max_color = -1;
	count_max = 0;


	if(array_x.size() == 0)
	{
		hist->max_color = 0;
		hist->min_color = 0;
		hist->count = 0;
		hist->percentil_pixel = 0;
		hist->upper_color = 0;
		hist->lower_color = 0;
		return hist;
	}

	for( vector<int>::size_type i = 0; i < array_x.size();i++)
	{
		GET_PIXELRGB(image, array_x[i], array_y[i], 0, &p);
		mapColor[p]++;
		count++;
	}

	percentil_limit = (int)(count * percentil);

	hist->size = mapColor.size();

	int i = 0;
	for(MapTypeHist::const_iterator it = mapColor.begin(); it != mapColor.end(); it++, i++)
	{
		int key = it->first;
		int value = it->second;
		array_color.push_back(key);
		array_count.push_back(value);
	}

	for( int i = hist->size -1; i >= 0; i--)
	{
		if(count_min >= array_count[i] && array_count[i] != 0)
		{
			count_min = array_count.at(i);
			min_color = array_color.at(i);
		}
		if(count_max < array_count[i] && array_count[i] != 0)
		{
			count_max = array_count.at(i);
			max_color = array_color.at(i);
		}

		partial_count += array_count.at(i);

		if(partial_count <= percentil_limit)
		{
			hist->percentil_pixel = array_color.at(i);
		}
	}

	hist->max_color = max_color;
	hist->min_color = min_color;
	hist->count = count;
	hist->upper_color = array_color[array_color.size()-1];
	hist->lower_color = array_color[0];
	mapColor.clear();
	array_color.clear();

	return hist;
}
