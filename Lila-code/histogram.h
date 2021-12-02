#ifndef __HISTOGRAM__
#define __HISTOGRAM__
#include "custom_open_cv.h"
struct Histogram
{
	int size;
	int count;
	int min_color;
	int max_color;
	int percentil_pixel;
	int upper_color;
	int lower_color;
};

struct Histogram *calculate_histogram_from_region(IplImage *image, vector<int> &array_x, vector<int> &array_y);
struct Histogram *calculate_histogram_threshold(IplImage *image, vector<int> &array_x, vector<int> &array_y, float percentil);

#endif
