#include "Processing.h"

void Display(SDL_Surface *img) 
{
	SDL_Surface *screen = display_image(img);
        update_surface(screen, img);
        wait_for_keypressed();
}

SDL_Surface *GetSurface(SDL_Surface *img, int x, int y, int w, int h)
{
	SDL_Surface *res = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			Uint32 pixel = get_pixel(img, i + x, j + y);
			put_pixel(res, i, j, pixel);
		}
	}

	return res;
}

void GetGrid(char *imgPath)
{
	SDL_Surface *img = load_image(imgPath);
	/*make a copy of the src image (so it's unprocessed)*/
	SDL_Surface *cpy = copy_image(img);

	PreProcess(img, 2, 0, 0);

	int len = 0;
	Uint8 color = SDL_MapRGB(img -> format, 255, 0, 0);

	struct Component *comps = GetComponents(img,
			&len,
			img -> w,
			img -> h,
			20,
			20,
			0,
			1.5,
			0.7);

	struct Component *c;

	/*Get the grid (we suppose that the grid is the
	 * largest component in our image)*/
	int biggest = 0;
	int maxSize = -1;
	for (int i = 0; i < len; i++)
	{
		c = &comps[i];
		if ((int) c -> points -> size >= maxSize)
		{
			maxSize = c -> points -> size;
			biggest = i;
		}
	}

	c = &comps[biggest];

	/*Now we process the grid to detect and store
	 * the characters*/
	SDL_Surface *gridImg = CompToBMP(c, 1);
	SDL_SaveBMP(gridImg, "onlyGrid.bmp");

	SDL_Surface *grid = GetSurface(cpy, c -> box_origin_x, c -> box_origin_y, c -> width, c -> height);

	int gridX = c -> box_origin_x;
	int gridY = c -> box_origin_y;

	PreProcess(grid, 2, 0, 0);

	for (int i = 0; i < gridImg -> w; i++)
	{
		for (int j = 0; j < gridImg -> h; j++)
		{
			Uint32 whitePx = SDL_MapRGB(grid -> format, 255, 255, 255);
			Uint32 pixel = get_pixel(gridImg, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, grid -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				put_pixel(grid, i, j, whitePx);
			}
		}
	}

	SDL_SaveBMP(grid, "mask.bmp");

	struct Component *digits = GetComponents(grid,
                        &len,
                        grid -> w,
                        grid -> h,
                        0,
                        0,
                        0,
                        1,
                        0);

	for (int i = 0; i < len; i++)
        {
                c = &digits[i];
		//DrawRectangle(grid, c -> box_origin_y, c -> box_origin_x, c -> height, c -> width, 4, color);
        }

	/*index of the digit in the grid*/
	int digitNb = 0;

	int cellW = grid -> w / 9;
	int cellH = grid -> h / 9;

	/*divide grid in 81 regions of width cellW and
	 * height cellH*/
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			/*foreach region:
			 * check if there is a component inside*/
			for (int k = 0; k < len; k++)
			{
				c = &digits[k];
				int x = c -> box_origin_x;
				int y = c -> box_origin_y;

				/*a component is in a region if
				 * its top left corner is in the region.
				 * a region w/o component is a blank cell
				 * is the grid.
				 */

				if (x > j * cellW &&
					y > i * cellH &&
					x < j * cellH + cellH &&
					y < i * cellW + cellW)
				{
					DrawRectangle(grid, i * cellH, j * cellW, cellH, cellW, 4, color);
					c -> id = digitNb;

					/*get the component in the main image (a new digit)
					 * the component top let corner is calculated by
					 * adding the coordinates of the grid.
					 * */
					SDL_Surface *digitToImg = GetSurface(cpy, 
							c -> box_origin_x + gridX,
							c -> box_origin_y + gridY,
							c -> width, 
							c -> height);

					/*use asprintf() to build the digit filename*/
					char *name;
					int e = asprintf(&name, "digits/%i%s", c -> id, ".bmp");
					/*rescale and save the digit*/
					SDL_SaveBMP(NearestNeighbourScale(digitToImg, 28, 28), name);

					/*use asprintf again to store the digit as a text file
					 * (build the file name)*/
					char *toTxt;
					e = asprintf(&toTxt, "digits/%i%s", c -> id, ".txt");
					
					/*use gocr for character recognition
					 * use fork() bc execlp exits the program
					 *we call gocr and save the digit as a text file
					 */
					int pid = fork();
					fflush(stdout);
					if (pid == 0)
					{
						execlp("gocr", "gocr", "-o", toTxt, name, (char *) NULL);
						exit(0);
					}
					else if (pid > 0)
					{
						wait(NULL);
					}
				}
			}

			digitNb += 1;
		}
	}

	/*allocate the memory for the grid (9 x 9 array)*/
	int *gridArr = malloc(81 * sizeof(int));

	/*set all values to -1*/
	for (int i = 0; i < 81; i++)
	{
		gridArr[i] = 0;
	}

	/*for each component in the digits component array
	 * get the corresponding text file and write the
	 * digit value at the correponding index in the grid*/
	for (int i = 0; i < len; i++)
	{
		char *path;
		c = &digits[i];
                int e = asprintf(&path, "digits/%i.txt", c -> id);
                FILE *fp = fopen(path, "r");
		if (fp && digits[i].id < 81)
		{
			int val = fgetc(fp) - '0';
			if (val == 47)
				val = 1;
                	gridArr[digits[i].id] = val;
		}
		fclose(fp);
	}

	FILE *fp = fopen("grid", "w");

	/*print/ save the final result*/
	for (int i = 0; i < 81; i++)
	{
		if (gridArr[i] == 0)
		{
			printf(".");
			fputc('.', fp);
		}
		else
		{
			printf("%i", gridArr[i]);
			fputc(gridArr[i] + '0', fp);
		}

		/*new line and space*/
		if ((i + 1) % 3 == 0)
		{
			printf(" ");
			putc(' ', fp);
		}

		if ((i + 1) % 9 == 0)
		{
			printf("\n");
			fputc('\n', fp);
		}
	}

	fclose(fp);

	Display(grid);
	SDL_SaveBMP(grid, "grid.bmp");
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		return 1;
	}
	GetGrid(argv[1]);
	return 0;
}
