#include "PreProcessing.h"

void Invert(SDL_Surface *img) // invert binary image
{
	Uint32 pixel;
	Uint8 r, g, b;

	for (int i = 0; i < img -> w; i++)
	{
		for (int j =  0; j < img -> h; j++)
		{
			pixel = get_pixel(img, i, j);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0) // black px so becomes white
			{
				pixel = SDL_MapRGB(img -> format, 255, 255, 255);
				put_pixel(img, i, j, pixel);
			}
			else // white becomes black
			{
				pixel = SDL_MapRGB(img -> format, 0, 0, 0);
				put_pixel(img, i, j, pixel);
			}
		}
	}
}

void Grayscale(SDL_Surface *image)
{
	Uint32 pixel;
	Uint8 r, g, b;
	Uint8 res;

	for (int i = 0; i < image -> w; i++)
	{
		for (int j = 0; j < image -> h; j++)
		{
			pixel = get_pixel(image, i, j);
			SDL_GetRGB(pixel, image -> format, &r, &g, &b);
			
			res = 21 * r / 100 + 72 * g / 100 + 7 * b / 100;
			pixel = SDL_MapRGB(image -> format, res, res, res);
			put_pixel(image, i, j, pixel);
		}
	}
}

Uint8 GetOtsuThreshold(SDL_Surface* image_surface)
{
	unsigned long histo[256];
	Uint8 r, g, b;

 	for (int x = 0; x < 256; x++)
		histo[x] = 0;

 	for (int i = 0; i < image_surface->h; i++)
 	{
	 	for (int j = 0; j < image_surface->w; j++)
	 	{	
		 	Uint32 pixel = get_pixel(image_surface, j, i);
		       	SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

		 	histo[r] += 1;
	 	}
 	}

 	unsigned long sum = 0, sum1 = 0, w1 = 0, w2 = 0;
 	unsigned long m1, m2;
 	float between = 0, max = 0;
 	Uint8 threshold1 = 0, threshold2 = 0;
 	unsigned long total = image_surface->h * image_surface->w;

 	for (int i = 0; i < 256; i++)
	 	sum += i * histo[i];

 	for (int i = 0; i < 256; i++)
 	{
	 	w1 += histo[i];
	 	if (w1 == 0)
		 	continue;

	 	w2 = total - w1;
	 	if (w2 == 0)
		 	break;

	 	sum1 += i * histo[i];
	 	m1 = sum1 / w1;
	 	m2 = (sum - sum1) / w2;
	 	between = w1 * w2 * (m1 - m2) * (m1 - m2);
	 	if (between >= max)
	 	{
		 	threshold1 = i;
		 	if (between > max)
			 	threshold2 = i;

		 	max = between;
	 	}
 	}

 	Uint8 threshold = (threshold1 + threshold2) / 2;
 	return threshold;
}

void Binarize(SDL_Surface* image_surface)
{

	
	Uint8 r, g, b;
	Uint32 pixel;
	/*
	for (int i = 0; i < image_surface -> w; i++)
	{
		for (int j = 0; j < image_surface -> h; j++)
		{
			pixel = get_pixel(image_surface, i, j);
                        SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                        if (b > 100)
                        {
                        	pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
                        	put_pixel(image_surface, i, j, pixel);
                        }
		}
	}
	*/
	
	Uint8 threshold = GetOtsuThreshold(image_surface);

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
                        SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

			if (r > threshold)
			{
				pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
                        	put_pixel(image_surface, j, i, pixel);
			}

			else
			{
				pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
                                put_pixel(image_surface, j, i, pixel);
			}
		}
	}
	//SDL_SaveBMP(image_surface, "test_remove_blue.bmp");
}

void PreProcess(SDL_Surface *img, int nbBlur, int nbErode, int nbDilate)
{
	for (int i = 0; i < nbBlur; i++)
		BoxBlur(img);
		
	Uint8 r, g, b;
	Uint32 pixel;
	
	for (int i = 0; i < img -> w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			pixel = get_pixel(img, i, j);
                        SDL_GetRGB(pixel,img->format,&r,&g,&b);
		}
	}

	Grayscale(img);
        Binarize(img);
        
        SDL_SaveBMP(img, "test.bmp");
        

	for (int i = 0; i < nbErode; i++)
		Erosion(img);

	for (int i = 0; i < nbDilate; i++)
                Dilation(img);
}
