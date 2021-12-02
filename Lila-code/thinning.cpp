#include "stdafx.h"
#include "custom_open_cv.h"

void Neiborhood(IplImage *image,int x, int y,int P[])
{
		int p2 ;
		// get_channel(x, y - 1,image,0)
		GET_PIXEL(image,x,y-1,&p2);
		
		int p3 ;
		//= get_channel(x + 1, y - 1,image,0);
		GET_PIXEL(image,x+1,y-1,&p3);

		int p4 ;
		//= get_channel(x + 1, y,image,0);
     	GET_PIXEL(image,x+1,y,&p4);

		int p5 ;
		//= get_channel(x + 1, y + 1,image,0);
	    GET_PIXEL(image,x+1,y+1,&p5);

		int p6 ;
		//= get_channel(x, y + 1,image,0)		
		GET_PIXEL(image,x,y+1,&p6);

		int p7 ;
		//= get_channel(x - 1, y + 1,image,0);
		GET_PIXEL(image,x-1,y+1,&p7);

		int p8 ;
		//= get_channel(x - 1, y,image,0);
		GET_PIXEL(image,x-1,y,&p8);

		int p9 ;
		//= get_channel(x - 1, y - 1,image,0)
		GET_PIXEL(image,x-1,y-1,&p9);


	P[0] = p2 == 255 ? 0 : 1;		// p2
	P[1] = p3 == 255 ? 0 : 1;	// p3
	P[2] = p4 == 255 ? 0 : 1;		// p4
	P[3] = p5 == 255 ? 0 : 1;	// p5
	P[4] = p6 == 255 ? 0 : 1;		// p6
	P[5] = p7 == 255 ? 0 : 1;	// p7
	P[6] = p8 == 255 ? 0 : 1;		// p8
	P[7] = p9 == 255 ? 0 : 1;	// p9
} 

void ChangePixels_A(int x, int y, IplImage *image, IplImage *imageAux)
{
	int valC = 1, valD = 1;

	    int p2 ;
		// get_channel(x, y - 1,image,0)
		GET_PIXEL(image,x,y-1,&p2);
		
		int p4 ;
		//= get_channel(x + 1, y,image,0);
     	GET_PIXEL(image,x+1,y,&p4);

		int p6 ;
		//= get_channel(x, y + 1,image,0)		
		GET_PIXEL(image,x,y+1,&p6);


		int p8 ;
		//= get_channel(x - 1, y,image,0);
		GET_PIXEL(image,x-1,y,&p8);



		valC = p2 == 255 ? 0 : 1 * // p2
			   p4 == 255 ? 0 : 1 * // p4
			   p6 == 255 ? 0 : 1;  // p6

		valD = p4 == 255 ? 0 : 1 * // p4
			   p6 == 255 ? 0 : 1 * // p6
			   p8 == 255 ? 0 : 1;  // p8

		if (valC == 0 && valD == 0) 
		{
			SET_PIXEL(imageAux, x, y, 255);
		}

		
}
void ChangePixels_B(int x, int y, IplImage *image, IplImage *imageAux)
{
	int valC = 1, valD = 1;

	    int p2 ;
		// get_channel(x, y - 1,image,0)
		GET_PIXEL(image,x,y-1,&p2);
		
		int p4 ;
		//= get_channel(x + 1, y,image,0);
     	GET_PIXEL(image,x+1,y,&p4);

		int p6 ;
		//= get_channel(x, y + 1,image,0)		
		GET_PIXEL(image,x,y+1,&p6);


		int p8 ;
		//= get_channel(x - 1, y,image,0);
		GET_PIXEL(image,x-1,y,&p8);



		valC = p2 == 255 ? 0 : 1 * // p2
			   p4 == 255 ? 0 : 1 * // p4
			   p8 == 255 ? 0 : 1;  // p8

		valD = p2 == 255 ? 0 : 1 * // p2
			   p6 == 255 ? 0 : 1 * // p6
			   p8 == 255 ? 0 : 1;  // p8

		if (valC == 0 && valD == 0 ) 
		{
			SET_PIXEL(imageAux, x, y, 255);
		}

		
}

IplImage * thinning(IplImage *imageOriginal)
{
	IplImage *image, *imageAux; 

	int width = imageOriginal->width;
	int height = imageOriginal->height;
	int depth = imageOriginal->depth;	
	int p;
	bool flag;

	image = createImage(width, height, 8, 1);
	imageAux = createImage(width, height, 8, 1);

	cvCvtColor(imageOriginal, image, CV_BGR2GRAY);
	cvCvtColor(imageOriginal, imageAux, CV_BGR2GRAY);

	// Binary image process
	cvThreshold(image, image, 100, 255, CV_THRESH_BINARY);
	cvThreshold(imageAux, imageAux, 100, 255, CV_THRESH_BINARY);

	do
	{
		flag = false;
		for (int y = 1; y < image->height - 1; y++)
		{
			for (int x = 1; x < image->width - 1; x++)
			{
				GET_PIXEL(image,x,y,&p);
				
				if (p == 0)
				{
					int P[8];
					Neiborhood(image, x, y, P);
					int NP = P[0] + P[1] + P[2] + P[3] + P[4] + P[5] + P[6]	+ P[7];
					int S = 0; 	// S(p1) = Número de transições de (0-1)
					if (NP >= 2 && NP <= 6)
					{
						for (int _X = 0; _X <= 7; _X++)
						{
							if (_X == 7)
							{
								if (P[7] == 0 && P[0] == 1)
								{
									S++;
								}
							}
							else if (P[_X] == 0 && P[_X + 1] == 1)
							{
								S++;
							}
					 }
						if (S == 1) 
						{
							ChangePixels_A(x, y,image, imageAux);//imagem fonte
							flag = true;
							
						}
					}
					
				}
			}
			
		}
		cvReleaseImage(&image);
		image = cvCloneImage(imageAux);

	    for (int y = 1; y < image->height - 1; y++)
		{
			for (int x = 1; x < image->width - 1; x++)
			{
				GET_PIXEL(image,x,y,&p);
				
				if (p == 0)
				{
					int P[8];
					Neiborhood(image, x, y, P);
					int NP = P[0] + P[1] + P[2] + P[3] + P[4] + P[5] + P[6]	+ P[7];
					int S = 0; 	// S(p1) = Número de transições de (0-1)
					if (NP >= 2 && NP <= 6)
					{
						for (int _X = 0; _X <= 7; _X++)
						{
							if (_X == 7)
							{
								if (P[7] == 0 && P[0] == 1)
								{
									S++;
								}
							}
							else if (P[_X] == 0 && P[_X + 1] == 1)
							{
								S++;
							}
					   }
						if (S == 1) 
						{
							ChangePixels_B(x, y,image, imageAux);//imagem fonte
							flag = true;
							
						}
					}
				}
			}
		}
		cvReleaseImage(&image);
	    image = cvCloneImage(imageAux);

	} while (flag);

	cvReleaseImage(&imageAux);

	return image;
}
