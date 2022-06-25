#include "Component.h"

//TODO : change way components saved, so i dont use the src img  -> DONE
void SaveComponentToBMP_2(struct Component *c, char *name, int offset)
{
	//int offset = 40;// so digits centered

	SDL_Surface *img = SDL_CreateRGBSurface(SDL_HWSURFACE, c -> width + offset, c -> height + offset, 32, 0, 0, 0, 0);

	Uint32 whitePixel = SDL_MapRGB(img -> format, 255, 255, 255);
	Uint32 blackPixel = SDL_MapRGB(img -> format, 0, 0, 0);

	for (int i = 0; i < img -> w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			put_pixel(img, i, j, whitePixel);
		}
	}

	for (size_t i = 0; i < c -> points -> size; i += 2)
	{
		int x = (c -> points -> data)[i + 1] - c -> box_origin_y;
		int y = (c -> points -> data)[i] - c -> box_origin_x;
		put_pixel(img, (y + offset / 2), x + (offset / 2), blackPixel);
	}

//	SDL_Surface *tmp = ResizeToFit(img, 100, 100);
	

	//SDL_SaveBMP(NearestNeighbourScale(img, 28, 28), name);


	SDL_SaveBMP(img, name);
	//SDL_FreeSurface(tmp);
	SDL_FreeSurface(img);
}

SDL_Surface *CompToBMP(struct Component *c, int offset)
{
        //int offset = 40;// so digits centered

        SDL_Surface *img = SDL_CreateRGBSurface(SDL_HWSURFACE, c -> width + offset, c -> height + offset, 32, 0, 0, 0, 0);

        Uint32 whitePixel = SDL_MapRGB(img -> format, 255, 255, 255);
        Uint32 blackPixel = SDL_MapRGB(img -> format, 0, 0, 0);

        for (int i = 0; i < img -> w; i++)
        {
                for (int j = 0; j < img -> h; j++)
                {
                        put_pixel(img, i, j, whitePixel);
                }
        }

        for (size_t i = 0; i < c -> points -> size; i += 2)
        {
                int x = (c -> points -> data)[i + 1] - c -> box_origin_y;
                int y = (c -> points -> data)[i] - c -> box_origin_x;
                put_pixel(img, (y + offset / 2), x + (offset / 2), blackPixel);
        }

//      SDL_Surface *tmp = ResizeToFit(img, 100, 100);


        //SDL_SaveBMP(NearestNeighbourScale(img, 28, 28), name);


        //SDL_SaveBMP(img, name);
        //SDL_FreeSurface(tmp);
        //SDL_FreeSurface(img);
	//
	return img;
}

struct Component *GetComponent(SDL_Surface *img, int x, int y)
{
	struct Component *c = malloc(sizeof(struct Component));
	c -> points = vector_new();

	Uint32 pixel;
	Uint32 redPixel = SDL_MapRGB(img -> format, 255, 0, 0);
	Uint8 r, g, b;

	struct vector *q = vector_new();

	// correction :vector used as a stack here (not as a queue "q")
	vector_push(q, x);
	vector_push(q, y);

	put_pixel(img, x, y, redPixel);

	// store extreme points from component; useful to get the height and width
	
	int lm_x = img -> w - 1, lm_y = 0; // lm : leftmost
	int rm_x = 0, rm_y = 0;
	int tm_x = 0, tm_y = img -> h - 1;
	int bm_x = 0, bm_y = 0;
	//

	while (vector_isEmpty(q) == 0)
	{
		vector_pop(q, &y);
		vector_pop(q, &x);

		if (x < lm_x)
		{
			lm_x = x;
			lm_y = y;
		}

		if (x > rm_x)
		{
			rm_x = x;
			rm_y = y;
		}

		if (y < tm_y)
		{
			tm_y = y;
			tm_x = x;
		}

		if (y > bm_y)
		{
			bm_y = y;
			bm_x = x;
		}

		vector_push(c -> points, x);
		vector_push(c -> points, y);
		
		// TODO : use one loop 

		if (x + 1 < img -> w)
		{
			pixel = get_pixel(img, x + 1, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				vector_push(q, x + 1);
				vector_push(q, y);
				put_pixel(img, x + 1, y, redPixel);
			}
		}

		if (x - 1 >= 0)
		{
			pixel = get_pixel(img, x - 1, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				vector_push(q, x - 1);
				vector_push(q, y);
				put_pixel(img, x - 1, y, redPixel);

			}
		}

		if (y + 1 < img -> h)
		{
			pixel = get_pixel(img, x, y + 1);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				vector_push(q, x);
				vector_push(q, y + 1);
				put_pixel(img, x, y + 1, redPixel);

			}
		}

		if (y - 1 >= 0)
		{
			pixel = get_pixel(img, x, y - 1);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				vector_push(q, x);
				vector_push(q, y - 1);
				put_pixel(img, x, y - 1, redPixel);

			}
		}
	}

	c -> rightmost_x = rm_x;
	c -> leftmost_x = lm_x;
	c -> topmost_x = tm_x;
	c -> bottommost_x = bm_x;

	c -> rightmost_y = rm_y;
        c -> leftmost_y = lm_y;
        c -> topmost_y = tm_y;
        c -> bottommost_y = bm_y;

	c ->  height = c -> bottommost_y - c -> topmost_y;
        c ->  width = c -> rightmost_x - c -> leftmost_x;

        c -> box_origin_x = c -> leftmost_x;
        c -> box_origin_y = c -> topmost_y;

	free(q);

	return c;
}

struct Component *GetComponents(SDL_Surface *img,
                int *len, int max_h, int max_w, 
                int min_h, int min_w, int min_size,
		float max_ratio, float min_ratio)
{
	SDL_Surface *img_copy = copy_image(img);
	
	Uint32 pixel;
	Uint8 r, g, b;

	//int MAX_NB = 100;

	struct Component *components = malloc(MAX_LEN * sizeof(struct Component)); // array of components : TODO : find a way to get adjustable size

	int count = 0; // index to loop over components array

	Uint32 color = SDL_MapRGB(img -> format, 0, 255, 0);

	for (int i = 0; i < img -> w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			pixel = get_pixel(img, i, j);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && count < MAX_LEN)
			{
				struct Component *c = GetComponent(img, i, j);

				// check component size
				if (c -> points -> size > (size_t) min_size 
					&& c -> height < max_h 
					&& c -> height > min_h
					&& c -> width < max_w 
					&& c -> width > min_w)
				{
					float ratio = ((float)(c -> width) / ((float) c -> height));
		                        //check aspect ratio
					if (ratio < max_ratio && ratio > min_ratio)
					{
						components[count] = *c;

						DrawRectangle(img_copy, 
								c -> box_origin_y, 
								c -> box_origin_x, 
								c -> height, 
								c ->  width, 
								4, 
								color);
						
						c -> id = count;
						count++;
					}
				}
			}
		}
	}

	SDL_SaveBMP(img_copy, "res.bmp");
	SDL_FreeSurface(img_copy);

	*len = count;

	return components;
}


double AngleBetweenComponents(struct Component *c1, struct Component *c2, struct Component *c3)
{
        // computes angle between vectors c1 -> c2 and c1 -> c3
        // plate numbers are usually colinear

        int p1_x = c1 -> topmost_x, p1_y = c1 -> topmost_y;
        int p2_x = c2 -> topmost_x, p2_y = c2 -> topmost_y;
        int p3_x = c3 -> topmost_x, p3_y = c3 -> topmost_y;

        double angle = (atan2(p3_y - p1_y, p3_x - p1_x) - atan2(p2_y - p1_y, p2_x - p1_x)) * 180 / M_PI;

        return angle;

}

double AngleBetweenComponents_2(struct Component *c1, struct Component *c2, struct Component *c3)
{
        // computes angle between vectors c1 -> c2 and c1 -> c3
        // plate numbers are usually colinear

        int p1_x = c1 -> bottommost_x, p1_y = c1 -> bottommost_y;
        int p2_x = c2 -> bottommost_x, p2_y = c2 -> bottommost_y;
        int p3_x = c3 -> bottommost_x, p3_y = c3 -> bottommost_y;

        double angle = (atan2(p3_y - p1_y, p3_x - p1_x) - atan2(p2_y - p1_y, p2_x - p1_x)) * 180 / M_PI;

        return angle;

}

struct vector *GetColinearComponents(struct Component *components, int *len, int min_angle)
{
        struct vector *current_cluster = vector_new();

        for (int i = 0; i < *len; i++)
        {
                for (int j = i + 1; j < *len; j++)
                {
                        for (int k = j + 1; k < *len; k++)
                        {
                                double angle = AngleBetweenComponents(&components[i], &components[j], &components[k]);

				double angle_2 =  AngleBetweenComponents_2(&components[i], &components[j], &components[k]);
                                if (fabs(angle) <= min_angle && fabs(angle_2) < 5)
                                {
                                        vector_push(current_cluster, components[k].id);
                                        //printf("%f\n", angle);
                                        //printf("colinear !\n");

                                        if (current_cluster -> size == 5)
                                                break;
                                }
                        }

                        if (current_cluster -> size == 5)
                        {
                                vector_push(current_cluster, components[j].id);
                                break;
                        }
                }

                if (current_cluster -> size == 6)
                {
                        vector_push(current_cluster, components[i].id);
                        break;
                }

                current_cluster -> size = 0;
        }
        return current_cluster;
}

// sort components (according to their x pos) to get license plate numbers in the correct order
void Swap(int *x, int *y)
{
        int temp = *x;
        *x = *y;
        *y = temp;
}

// sort component vector to store plate digits in the right order (order by position on x axis)
void SortComponentVector(struct vector *v, struct Component *components, int len)
{
        int *data = v -> data;
        int i, j, min_index;

        for (i = 0; i < len - 1; i++)
        {
                min_index = i;

                for (j = i + 1; j < len; j++)
                {
                        if (components[data[j]].box_origin_x < components[data[min_index]].box_origin_x)
                        {
                                min_index = j;
                        }
                }

                Swap(&v -> data[min_index], &v -> data[i]);
        }
}

int min(int x, int y)
{
	return x < y ? x : y;
}

int max(int x, int y)
{
	return x > y ? x : y;
}

struct Component *MergeComponents(struct Component *c1, struct Component *c2)
{
	struct Component *c = malloc(sizeof(struct Component));
	c -> points = vector_new();

	c -> box_origin_x = min(c1 -> box_origin_x, c2 -> box_origin_x);
	c -> box_origin_y = min(c1 -> box_origin_y, c2 -> box_origin_y);

	c -> rightmost_x = max(c1 -> rightmost_x, c2 -> rightmost_x);
	c -> rightmost_y = max(c1 -> rightmost_y, c2 -> rightmost_y);

	c -> leftmost_x = min(c1 -> leftmost_x, c2 -> leftmost_x);
        c -> leftmost_y = min(c1 -> leftmost_y, c2 -> leftmost_y);

	c -> topmost_x = min(c1 -> topmost_x, c2 -> topmost_x);
        c -> topmost_y = min(c1 -> topmost_y, c2 -> topmost_y);

	c -> bottommost_x = max(c1 -> bottommost_x, c2 -> bottommost_x);
        c -> bottommost_y = max(c1 -> bottommost_y, c2 -> bottommost_y);


	c ->  height = c -> bottommost_y - c -> topmost_y;
        c ->  width = c -> rightmost_x - c -> leftmost_x;
	

	for (size_t i = 0; i < c1 -> points -> size; i++)
	{
		vector_push(c -> points, (c1 -> points -> data)[i]);
	}

	for (size_t i = 0; i < c2 -> points -> size; i++)
	{
		vector_push(c -> points, (c2 -> points -> data)[i]);
	}

	return c;

}

int Overlap(struct Component *c1, struct Component *c2)
{
	int c1_x1, c1_y1;
	int c1_x2, c1_y2;

	int c2_x1, c2_y1;
        int c2_x2, c2_y2;

	c1_x1 = c1 -> box_origin_x;
	c1_y1 = c1 -> box_origin_y;

	c1_x2 = c1 -> box_origin_x + c1 -> width;
	c1_y2 = c1 -> box_origin_y + c1 -> height;

	c2_x1 = c2 -> box_origin_x;
        c2_y1 = c2 -> box_origin_y;

        c2_x2 = c2 -> box_origin_x + c2 -> width;
        c2_y2 = c2 -> box_origin_y + c2 -> height;

	if (!(c1_x1 < c2_x2))
		return 0;

	if (!(c1_x2 > c2_x1))
		return 0;

	if (!(c1_y1 < c2_y2))
		return 0;

	if (!(c1_y2 > c2_y1))
		return 0;

	return 1;
}

void free_component(struct Component *c)
{
	vector_free(c -> points);
	free(c);
}
