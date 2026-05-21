#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* vector_new(size_t item_size, size_t capacity) {
    void* ptr = 0;
    size_t size = item_size * capacity + sizeof(vector_header);
    vector_header* h = malloc(size);
    if (h) {
        h->capacity = capacity;
        h->size = 0;
        h->item_size = item_size;
        ptr = h + 1;
    }
    return ptr;
}

void* push(void* vec, void* item) {
    vector_header* h = vector_header(vec);
    if (h->size == h->capacity) {
        size_t new_capacity = h->capacity * 2;
        size_t total_size = sizeof(vector_header) + new_capacity * h->item_size;
        vector_header* new_header = realloc(h, total_size);
        if (!new_header) return NULL;
        new_header->capacity = new_capacity;
        vec = (void*)(new_header + 1);
        h = new_header;
    }
    void* dest = (char*)vec + h->size * h->item_size;
    memcpy(dest, item, h->item_size);
    h->size += 1;
    return vec;
}
