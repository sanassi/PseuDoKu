#include "Rescaling.h"

SDL_Surface *NearestNeighbourScale(SDL_Surface *src, int newWidth, int newHeight)
{
	if (newWidth == 0 || newHeight == 0)
	{
		errx(1, "Rescaling error : new size null");
	}

	SDL_Surface *res = SDL_CreateRGBSurface(SDL_HWSURFACE, newWidth, newHeight, 32, 0, 0, 0, 0);

	int w1 = src -> w;
	int h1 = src -> h;

	int xRatio = ((w1 << 16) / newWidth) + 1;
	int yRatio = ((h1 << 16) / newHeight) + 1;

	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			int x2, y2;

			x2 = ((j * xRatio) >> 16);
			y2 = ((i * yRatio) >> 16);

			Uint32 pixel = get_pixel(src, x2, y2);
			put_pixel(res, j, i, pixel);
		}
	}
	
	// idk why but code turns images blue
	return res;
}

SDL_Surface *ResizeToFit(SDL_Surface *img, float maxHeight, float maxWidth)
{
	float scaleFactor =  maxHeight / img -> h;

	if (maxWidth / img -> w < maxHeight / img -> h)
		scaleFactor = maxWidth / img -> w;

	return NearestNeighbourScale(img, img -> w * scaleFactor, img -> h * scaleFactor);
}
