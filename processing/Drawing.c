#include "Drawing.h"

void DrawLine(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color)
{
	// bresenham line algorithm
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx - dy, e2, x2, y2;
	float ed = dx + dy == 0 ? 1 : sqrt((float) dx * dx + (float) dy * dy);


	Uint32 pixel = pixel_color;

	for (wd = (wd + 1) / 2 ; ; )
	{
		if (x0 >= 0 && y0 >= 0 && x0 < img -> h && y0 < img -> w)
		{
			put_pixel(img, y0, x0, pixel);
		}

		e2 = err;
		x2 = x0;

		if (2 * e2 >= -dx)
		{
			for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
			{
				if (x0 >= 0 && x0 < img -> h && (y2 + sy) >= 0 && (y2 + sy) < img -> w)
				{
					put_pixel(img, (y2 += sy), x0, pixel);
				}
			}

			if (x0 == x1)
			{
				break;
			}

			e2 = err;
			err -= dy;
			x0 += sx;
		}

		if (2 * e2 <= dy)
		{
			for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
			{
				if ((x2 + sx >= 0 && x2 + sx < img -> h) && (y0 >= 0 && y0 < img -> w))
				{
					put_pixel(img, y0, x2 += sx, pixel);
				}
			}

			if (y0 == y1)
			{
				break;
			}

			err += dx;
			y0 += sy;
		}


	}
}

// ici x ordonee ; y abscisse
void DrawFillCircle(SDL_Surface *img, int center_x, int center_y, int r, Uint32 color)
{
	int x = 0;
	int y = r;
	int m = 5 - 4 * r;

	while (x <= y)
	{
		DrawLine(img, center_x - y, center_y - x, center_x + y, center_y - x, 1, color);
		DrawLine(img, center_x - y, center_y + x, center_x + y, center_y + x, 1, color);

		if (m > 0)
		{
			DrawLine(img, center_x - x, center_y - y, center_x + x, center_y - y, 1, color);
	                DrawLine(img, center_x - x, center_y + y, center_x + x, center_y + y, 1, color);
			y -= 1;
			m -= 8 * y;
		}

		x += 1;
		m += 8 * x + 4;
	}
}

void DrawRectangle(SDL_Surface *img, int x, int y, int h, int w, int lineWidth, Uint32 lineColor)
{
	// x -> h; y -> w
	
	DrawLine(img, x, y, x, y + w, (float) lineWidth, lineColor);
	DrawLine(img, x, y, x + h, y, (float) lineWidth, lineColor);
	DrawLine(img, x + h, y, x + h, y + w, (float) lineWidth, lineColor);
	DrawLine(img, x, y + w, x + h, y + w, (float) lineWidth, lineColor);


}


