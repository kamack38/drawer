#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

#define MAX_COLOR 1

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel_t;

typedef struct {
    int width;
    int height;
    pixel_t** bitmap;
} image_t;

extern const pixel_t WRONG_PIXEL;

pixel_t** bitmap_alloc(int width, int height);
int write_image(image_t image, FILE* output, int binary);
pixel_t parse_color(char* str, size_t len);
int pixel_eq(pixel_t a, pixel_t b);
#endif  // IMAGE_H
