CC = gcc
CFLAGS = -Wall  -Wextra -pedantic -std=c99 -O3
CPPFLAGS= `pkg-config --cflags gtk+-3.0  sdl` -MMD
LDLIBS= `pkg-config --libs gtk+-3.0 sdl SDL_image` -lm
TARGET = src/bin/PseuDoKu

MAIN = src/main.o
OBJ = $(patsubst %.c, %.o, $(wildcard src/*/*.c))

HEADERS = $(wildcard src/*/*.h)
INC_FLAGS = $(addprefix -I, $(dir $(wildcard src/*/)))

CFLAGS += $(INC_FLAGS)

all: release

release: CFLAGS += -O2
release: CPPFLAGS += -DNDBUG
release: $(TARGET)

$(TARGET): $(MAIN) $(OBJ)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ): %.o: %.c $(HEADERS)

.PHONY: clean

clean:
	$(RM) $(TARGET)
	$(RM) $(MAIN)
	$(RM) $(OBJ)
