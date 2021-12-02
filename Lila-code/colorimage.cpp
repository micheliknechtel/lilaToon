#include "stdafx.h"
#include "region.h"
#include "histogram.h"
#include "custom_open_cv.h"

IplImage *MakeColorImage(IplImage* image,
						 int numberRegions,
						 map<int, struct Region> &regions)
{
	struct Histogram *hist;
	IplImage *imageColor;

	for (int i = 0;i < numberRegions;i++)
	{
		hist = calculate_histogram_from_region(image, regions[i].fill_arr_x, regions[i].fill_arr_y);
		regions[i].paint_color = hist->max_color;
		free(hist);
	}	

	imageColor = createImageRGB(image->width, image->height);	
	cvRectangle(imageColor, cvPoint(0,0), cvPoint(image->width, image->height), cvScalar(255,255,255),-1);

	for (int r = 1;r < numberRegions;r++)
	{
		for(vector<int>::size_type i = 0;i < regions[r].contour_arr_x.size();i++)
		{
			SET_PIXEL4INT(imageColor,regions[r].contour_arr_x.at(i), regions[r].contour_arr_y.at(i), regions[r].paint_color);
		}
		for(vector<int>::size_type i = 0;i < regions[r].fill_arr_x.size();i++)
		{
			SET_PIXEL4INT(imageColor,regions[r].fill_arr_x.at(i), regions[r].fill_arr_y.at(i), regions[r].paint_color);
		}
		for(vector<vector<int>>::size_type i = 0;i < regions[r].branch_arr_x.size();i++)
		{
			for(vector<int>::size_type b = 0;b < regions[r].branch_arr_x.at(i).size();b++)
			{
				SET_PIXEL4INT(imageColor,regions[r].branch_arr_x.at(i).at(b),regions[r].branch_arr_y.at(i).at(b), regions[r].paint_color);
			}
		}
		for(vector<vector<int>>::size_type i = 0;i < regions[r].isolated_arr_x.size();i++)
		{
			for(vector<int>::size_type b = 0;b < regions[r].isolated_arr_x.at(i).size();b++)
			{
				SET_PIXEL4INT(imageColor, regions[r].isolated_arr_x.at(i).at(b), regions[r].isolated_arr_y.at(i).at(b), regions[r].paint_color);
			}
		}
	}	

	return imageColor;
}
