#include "Vector.h"
#include <err.h>

struct vector *vector_new()
{
	struct vector *v = malloc(sizeof(struct vector));
	v -> capacity = 1;
	v -> size = 0;
	v -> data = malloc((v -> capacity) * sizeof(int));

	return v;
}

void vector_free(struct vector *v)
{
	v -> size = 0;
	v -> data = NULL;
	v -> capacity = 0;

	free(v -> data);
	free(v);
}

void double_capacity(struct vector *v)
{
	int *newDataPtr;
	if ((newDataPtr = (int *)realloc(v -> data, 2 * (v -> capacity) * sizeof(int))) != NULL)
	{
		v -> data = newDataPtr; 
		v -> capacity = v -> capacity * 2;
	}
	else
	{
		errx(1, "Vector Double Capacity : Not Enough Memory.");
	}
}

void vector_push(struct vector *v, int x)
{
	if (v -> size == v -> capacity)
	{
		double_capacity(v);
	}
	
	v -> size += 1;
	v -> data[(int) v -> size - 1] = x;
}

//remove and return last elt of vector. if empty -> return 0 else store elt in x and return 1
int vector_pop(struct vector *v, int *x)
{
	if (v -> size == 0)
		return 0;

	*x = v -> data[v -> size - 1];

	v -> size -= 1;

	return 1;

}

int vector_isEmpty(struct vector *v)
{
	if (v -> size == 0)
		return 1;

	return 0;
}

void print_vector(struct vector *v)
{
	for (size_t i = 0; i < v -> size; i++)
	{
		printf("%i ", (v -> data)[i]);
	}
}
