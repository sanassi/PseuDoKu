#ifndef HOUGH_H_
#define HOUGH_H_

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <err.h>
#include <math.h>
#include "SdlHelperFunctions.h"
#include <float.h>
#include "Drawing.h"

void HoughTransform(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color);

#endif
