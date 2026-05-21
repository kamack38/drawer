#ifndef DRAW_H
#define DRAW_H

#include "image.h"

typedef enum {
    RECTANGLE,
    ELLIPSE,
    POLYGON,
} object_t;

typedef struct {
    int x, y;
} point_t;

void draw_triangle(image_t image, point_t points[3], pixel_t color);
void draw_rectangle(image_t image, point_t centre, int width, int height, pixel_t color,
                    double theta);
void draw_polygon(image_t image, point_t* points, size_t pointc, pixel_t color);
void draw_ellipse(image_t image, point_t centre, int width, int height, pixel_t color,
                  double theta);
object_t parse_object(char* str, size_t len);
int draw_pixel(image_t image, int x, int y, pixel_t color);
#endif  // DRAW_H
