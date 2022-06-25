#ifndef MORPH_H_
#define MORPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <err.h>
#include <math.h>
#include "SdlHelperFunctions.h"
#include <float.h>

void Erosion(SDL_Surface *img);

void Dilation(SDL_Surface *img);

void Opening(SDL_Surface *img);

void Closing(SDL_Surface *img);
#endif
