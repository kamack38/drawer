#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t capacity;
    size_t item_size;
} vector_header;

#define INITIAL_CAPACITY 16

#define vector(T) (T*)vector_new(sizeof(T), INITIAL_CAPACITY)
#define vector_header(v) (((vector_header*)(v)) - 1)
#define vector_free(v) free(vector_header(v))
#define length(v) (vector_header(v)->size)

void* vector_new(size_t item_size, size_t capacity);
void* push(void* vec, void* item);
#endif  // VECTOR_H
