#include "Hough.h"

void HoughTransform(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color)
{
	if (num_rhos == 0 || num_thetas == 0)
	{
		errx(1, "Errror Hough : incorrect parameters\n");
		return ;
	}

	int d = (int) (sqrt((edge_image -> h) * (edge_image -> h) + (edge_image -> w) * (edge_image -> w)));

	double d_theta =  181 / num_thetas;
//	double d_rho = (int) (2 * d + 1) / num_rhos;

	double thetas[181];
	double sin_thetas[181];
	double cos_thetas[181];
	double rhos[2 * d + 1];

	int step = 0;
	for (int i = 0; i < 181; i++)
	{
		thetas[i] = step;

		sin_thetas[i] = sin(thetas[i] * (M_PI / 180));
		cos_thetas[i] = cos(thetas[i] * (M_PI / 180));
		step += d_theta;
	}

	step = -d;


	for (int i = 0; i < 2 * d + 1; i++)
	{
		rhos[i] = step + i;
	}

//	printf("%f    %f\n", rhos[0], rhos[2 * d]);

	// init accumulator
	int accumulator[2 * d + 1][181];

	for (int i = 0; i < 2 * d + 1; i++)
	{
		for (int j = 0; j < 181; j++)
		{
			accumulator[i][j] = 0;
		}
	}


	// begin hough
	
	Uint32 pixel;
	Uint8 r, g, _b;

	int theta;
	double rho;
	
	int half_w = edge_image -> w / 2;
	int half_h = edge_image -> h / 2;

	int rho_index = 0;


	for (int y = 0; y < edge_image -> h; y++)
	{
		for (int x = 0; x < edge_image -> w; x++)
		{
			pixel = get_pixel(edge_image, x, y);
			SDL_GetRGB(pixel, src -> format, &r, &g, &_b);

			double edge_point_1 = x - half_w;
			double edge_point_2 = y - half_h;

			if (r == 255 && g == 255 && _b == 255) // white pixel => edge pixel
			{
				for (int k = 0; k < 181; k += d_theta)
				{
					rho = (edge_point_1 * cos_thetas[k]) + (edge_point_2 * sin_thetas[k]);

					theta = thetas[k];

					double min_rho_abs = DBL_MAX;

					for (int l = 0; l <  2 * d + 1; l++)
					{
						// get rho index 
						if (fabs(rho - rhos[l]) < min_rho_abs)
						{
							rho_index = l;
							min_rho_abs = fabs(rho - rhos[l]);
						}
						if (rhos[l] > rho) // reduce hough run time: rhos array in increasing order => break if current val > rho
							break;
					}

					accumulator[rho_index][theta]++;
				}
			}
		}
	}

	double a, b, x0, y0;

	double x1, y1, x2, y2;
	
	// set line color 
	Uint32 color;
	
	if (!strcmp(line_color, "red"))
	{
		color = SDL_MapRGB(src -> format, 255, 0, 0);
	}
	
	else if (!strcmp(line_color, "green"))
	{
		color = SDL_MapRGB(src -> format, 0, 255, 0);
	}
	
	else if (!strcmp(line_color, "blue"))
	{
		color = SDL_MapRGB(src -> format, 0, 0, 255);
	}
	
	else // white
	{
		color = SDL_MapRGB(src -> format, 255, 255, 255);
	}


	for (int y = 0; y < 2 * d + 1; y++)
	{
		for (int x = 0; x < 181; x++)
		{
			if (accumulator[y][x] > threshold)
			{
				rho = rhos[y];
				theta = thetas[x];

				if (abs(theta - 45) < 10 || abs(theta - 135) < 20) // skip diagonal lines
					continue;

				if (abs(theta - 90) > 10 && abs(theta) > 10) // skewed line => skip 
					continue;

				a = cos(theta * (M_PI / 180));
				b = sin(theta * (M_PI / 180));
				
				x0 = (a * rho) + half_w;
				y0 = (b * rho) + half_h;

				x1 = (int) (x0 + 1000 * (-b));
				y1 = (int) (y0 + 1000 * (a));
				x2 = (int) (x0 - 1000 * (-b));
				y2 = (int) (y0 - 1000 * (a));

				DrawLine(src, y1, x1, y2, x2, 10, color);
			}
		}
	}
}
