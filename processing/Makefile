CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= `pkg-config --cflags glib-2.0 `-Wall -Wextra -Werror  -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image glib-2.0` -lm

SRC = main.o PreProcessing.c SdlHelperFunctions.c Sobel.c Blur.c Rescaling.c Vector.c  Drawing.c Morph.c Component.c Hough.c

main : main.o PreProcessing.c SdlHelperFunctions.c Sobel.c Blur.c Rescaling.c Vector.c Drawing.c Morph.c Component.c Hough.c

OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

clean :
	rm *.o *.d 
