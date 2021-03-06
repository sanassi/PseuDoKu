#ifndef SDLHELPERFUNCTIONS_H_
#define SDLHELPERFUNCTIONS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

void init_sdl();

SDL_Surface* load_image(char *path);

SDL_Surface* display_image(SDL_Surface *img);

void wait_for_keypressed();

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void update_surface(SDL_Surface* screen, SDL_Surface* image);

void SDL_FreeSurface(SDL_Surface* image_surface);

SDL_Surface* copy_image(SDL_Surface *img);

#endif
