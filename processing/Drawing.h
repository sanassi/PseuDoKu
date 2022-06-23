#ifndef DRAWING_H_
#define DRAWING_H_

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <math.h>
#include "SdlHelperFunctions.h"

void DrawLine(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color);

void DrawRectangle(SDL_Surface *img, int x, int y, int h, int w, int lineWidth, Uint32 lineColor);

void DrawFillCircle(SDL_Surface *img, int center_x, int center_y, int r, Uint32 color);

#endif
