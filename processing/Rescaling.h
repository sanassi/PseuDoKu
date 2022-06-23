#ifndef RESCALING_H_
#define RESCALING_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "SdlHelperFunctions.h"
#include <err.h>

SDL_Surface *NearestNeighbourScale(SDL_Surface *src, int newWidth, int newHeight);

SDL_Surface *ResizeToFit(SDL_Surface *img, float maxHeight, float maxWidth);

#endif
