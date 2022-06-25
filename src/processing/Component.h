#ifndef COMPONENT_H_
#define COMPONENT_H_


#include <stdio.h>
#include <stdlib.h>
#include "Sobel.h"
#include "Blur.h"
#include "PreProcessing.h"
#include "SdlHelperFunctions.h"
#include "Rescaling.h"
//#include "Floodfill.h"
#include "Morph.h"
#include "Drawing.h"
#include "Vector.h"
//#include "Hough.h"
#include <string.h>
#include <math.h>

struct Component
{
	int box_origin_x, box_origin_y; // bounding box coordinates

	int height, width;

	int id;


	// extreme points
	int leftmost_x, leftmost_y;
	int rightmost_x, rightmost_y;
	int topmost_x, topmost_y;
	int bottommost_x, bottommost_y;
	
	// pixel points of the component
	struct vector *points; // pixel points of the component
};

#define MAX_LEN 1000
SDL_Surface *CompToBMP(struct Component *c, int offset);

struct Component *GetComponent(SDL_Surface *img, int x, int y);

struct Component *MergeComponents(struct Component *c1, struct Component *c2);

void free_component(struct Component *c);

void SaveComponentToBMP_2(struct Component *c, char *name, int offset);

void SortComponentVector(struct vector *v, struct Component *components, int len);


//struct Component *GetComponents(SDL_Surface *img, int *len);

struct Component *GetComponents(SDL_Surface *img,
                int *len, int max_h, int max_w, int min_h, int min_w, int min_size,
		float max_ratio, float min_ratio);

double AngleBetweenComponents(struct Component *c1, struct Component *c2, struct Component *c3);

struct vector *GetColinearComponents(struct Component *components, int *len, int min_angle);

int Overlap(struct Component *c1, struct Component *c2);

#endif
