#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdio.h>

struct vector
{
	size_t capacity;
	size_t size;
	int *data;
};

struct vector *vector_new();

void vector_free(struct vector *v);

void double_capacity(struct vector *v);

void vector_push(struct vector *v, int x);

int vector_pop(struct vector *v, int *x);

void print_vector(struct vector *v);

int vector_isEmpty(struct vector *v);
#endif
