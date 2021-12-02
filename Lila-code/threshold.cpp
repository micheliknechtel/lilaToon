#include "stdafx.h"
#include "threshold.h"

IplImage *thresholdImage(IplImage *imagePhong, IplImage *imageColor, map<int, struct Region> &regions, int numberRegions, float percentil, int increase_pixel, bool withBorder)
{
	IplImage *image;

	struct Histogram *hist;
	int p0, p1, p2, g0, g1, g2;
	int x, y;

	image = createImageRGB(imagePhong->width, imagePhong->height);
	cvRectangle(image, cvPoint(0,0), cvPoint(image->width,image->height),cvScalar(255,255,255), -1);

	for(int r = 0;r < numberRegions;r++)
	{
		hist = calculate_histogram_threshold(imagePhong, regions[r].normal.fill_arr_x, regions[r].normal.fill_arr_y, percentil);
		for(vector<int>::size_type i = 0; i < regions[r].normal.fill_arr_x.size();i++)
		{
			x = regions[r].normal.fill_arr_x[i];
			y = regions[r].normal.fill_arr_y[i];

			GET_PIXELRGB(imagePhong, x, y, 0, &g0);
			GET_PIXELRGB(imagePhong, x, y, 1, &g1);
			GET_PIXELRGB(imagePhong, x, y, 2, &g2);
			GET_PIXELRGB(imageColor,x, y, 0, &p0);
			GET_PIXELRGB(imageColor,x, y, 1, &p1);
			GET_PIXELRGB(imageColor,x, y, 2, &p2);

			if(g0 < hist->percentil_pixel)
			{
				p0 = p0 - increase_pixel - (hist->upper_color-hist->lower_color)/2;
				p1 = p1 - increase_pixel - (hist->upper_color-hist->lower_color)/2;
				p2 = p2 - increase_pixel - (hist->upper_color-hist->lower_color)/2;

				p0 = p0 < 0? 0:p0;
				p1 = p1 < 0? 0:p1;
				p2 = p2 < 0? 0:p2;
			}
			SET_PIXELRGB(image, x, y, p0, 0);
			SET_PIXELRGB(image, x, y, p1, 1);
			SET_PIXELRGB(image, x, y, p2, 2);

		}

		/* Pinta a borda da image para melhorar a visualização da figura a ser apresentada */
		if(withBorder)
		{
			for(vector<int>::size_type i = 0; i < regions[r].contour_arr_x.size();i++)
			{
				x = regions[r].normal.contour_arr_x[i];
				y = regions[r].normal.contour_arr_y[i];

				GET_PIXELRGB(imagePhong, x, y, 0, &g0);
				GET_PIXELRGB(imagePhong, x, y, 1, &g1);
				GET_PIXELRGB(imagePhong, x, y, 2, &g2);
				GET_PIXELRGB(imageColor,x, y, 0, &p0);
				GET_PIXELRGB(imageColor,x, y, 1, &p1);
				GET_PIXELRGB(imageColor,x, y, 2, &p2);

				p0 = p0 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;
				p1 = p1 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;
				p2 = p2 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;

				p0 = p0 < 0? 0:p0;
				p1 = p1 < 0? 0:p1;
				p2 = p2 < 0? 0:p2;

				SET_PIXELRGB(image, x, y, p0, 0);
				SET_PIXELRGB(image, x, y, p1, 1);
				SET_PIXELRGB(image, x, y, p2, 2);
			}

			for(vector<vector<int>>::size_type b = 0; b < regions[r].branch_arr_x.size(); b++)
			{
				for(vector<int>::size_type i = 0; i < regions[r].branch_arr_x[b].size();i++)
				{
					x = regions[r].branch_arr_x[b].at(i);
					y = regions[r].branch_arr_y[b].at(i);

					GET_PIXELRGB(imagePhong, x, y, 0, &g0);
					GET_PIXELRGB(imagePhong, x, y, 1, &g1);
					GET_PIXELRGB(imagePhong, x, y, 2, &g2);
					GET_PIXELRGB(imageColor,x, y, 0, &p0);
					GET_PIXELRGB(imageColor,x, y, 1, &p1);
					GET_PIXELRGB(imageColor,x, y, 2, &p2);

					p0 = p0 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;
					p1 = p1 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;
					p2 = p2 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;

					p0 = p0 < 0? 0:p0;
					p1 = p1 < 0? 0:p1;
					p2 = p2 < 0? 0:p2;

					SET_PIXELRGB(image, x, y, p0, 0);
					SET_PIXELRGB(image, x, y, p1, 1);
					SET_PIXELRGB(image, x, y, p2, 2);
				}
			}

			for(vector<vector<int>>::size_type b = 0; b < regions[r].isolated_arr_x.size(); b++)
			{
				for(vector<int>::size_type i = 0; i < regions[r].isolated_arr_x[b].size();i++)
				{
					x = regions[r].isolated_arr_x[b].at(i);
					y = regions[r].isolated_arr_y[b].at(i);

					GET_PIXELRGB(imagePhong, x, y, 0, &g0);
					GET_PIXELRGB(imagePhong, x, y, 1, &g1);
					GET_PIXELRGB(imagePhong, x, y, 2, &g2);
					GET_PIXELRGB(imageColor,x, y, 0, &p0);
					GET_PIXELRGB(imageColor,x, y, 1, &p1);
					GET_PIXELRGB(imageColor,x, y, 2, &p2);

					p0 = p0 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;
					p1 = p1 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;
					p2 = p2 - increase_pixel * 2 - (hist->upper_color-hist->lower_color)/2;

					p0 = p0 < 0? 0:p0;
					p1 = p1 < 0? 0:p1;
					p2 = p2 < 0? 0:p2;

					SET_PIXELRGB(image, x, y, p0, 0);
					SET_PIXELRGB(image, x, y, p1, 1);
					SET_PIXELRGB(image, x, y, p2, 2);
				}
			}
		}

		free(hist);
	}


	return image;
}
