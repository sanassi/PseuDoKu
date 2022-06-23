#include "Morph.h"

// dialtion and erosion : use 3 x 3 struct elt filled w 1

// fuck it doesnt work ..

void Erosion(SDL_Surface *img)
{
	SDL_Surface *img_copy = copy_image(img);

	Uint32 pixel;
	Uint8 r, g, b;
	
	
	for (int i = 0; i < img -> w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			pixel = get_pixel(img_copy, i, j);
			SDL_GetRGB(pixel, img_copy -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				int count = 0;

				for (int x = -1; x < 1; x++)
				{
					for (int y = -1; y < 1; y++)
					{
						if ((i + x >= 0) && (i + x < (img -> w)) && (j + y >= 0) && (j + y < (img -> h)))
						{
							pixel = get_pixel(img_copy, i + x, j + y);
							SDL_GetRGB(pixel, img_copy->format, &r, &g, &b);

							if (r == 255)
							{
								count += 1;
							}
						}
						
					}
				}

				if (count > 1)
				{
					pixel = SDL_MapRGB(img -> format, 255, 255, 255);
					put_pixel(img, i, j, pixel);
				}
			}
		}	
	}

	

	free(img_copy);
}

void Dilation(SDL_Surface *img)
{
	SDL_Surface *img_copy = copy_image(img);

	Uint32 pixel;
	Uint8 r, g, b;
	
	for (int i = 0; i < img -> w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			pixel = get_pixel(img_copy, i, j);
			SDL_GetRGB(pixel, img_copy -> format, &r, &g, &b);

			if (r == 255 && g == 255 && b == 255)
			{
				int count = 0;

				for (int x = -1; x < 1; x++)
				{
					for (int y = -1; y < 1; y++)
					{
						if ((i + x >= 0) && (i + x < (img -> w)) && (j + y >= 0) && (j + y < (img -> h)))
						{

							pixel = get_pixel(img_copy, i + x, j + y);
							SDL_GetRGB(pixel, img_copy->format, &r, &g, &b);

							if (r == 0)
							{
								count = 1;
								break;
							}
						}
						
					}
				}

				if (count == 1) // at least on pixel on around a pixel off
				{
					pixel = SDL_MapRGB(img -> format, 0, 0, 0);
					put_pixel(img, i, j, pixel);
				}
			}
		}	
	}

	

	free(img_copy);
}

void Closing(SDL_Surface *img)
{
	Dilation(img);
	Erosion(img);
}

void Opening(SDL_Surface *img)
{
	Erosion(img);
	Dilation(img);
}


