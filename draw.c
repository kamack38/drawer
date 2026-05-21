#include "draw.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef long double ld;

object_t parse_object(char* str, size_t len) {
    if (len == 0) return -1;
    if (strncmp(str, "rectangle", len) == 0) {
        return RECTANGLE;
    } else if (strncmp(str, "ellipse", len) == 0) {
        return ELLIPSE;
    } else if (strncmp(str, "polygon", len) == 0) {
        return POLYGON;
    } else {
        return -1;
    }
}

int draw_pixel(image_t image, int x, int y, pixel_t color) {
    if (x < 0 || x >= image.width || y < 0 || y >= image.height) return -1;
    image.bitmap[y][x] = color;
    return 0;
}

int interpolate(int y, point_t start, point_t end) {
    if (start.y == end.y) return start.x;
    double starty = start.y, startx = start.x, endy = end.y, endx = end.x, dy = y;
    double res = startx + (dy - starty) * (endx - startx) / (endy - starty);
    return (int)floor(res + 0.5);
}

// Sorts by y lowest to highest
int point_cmp(const void* a, const void* b) {
    const point_t* p1 = (const point_t*)a;
    const point_t* p2 = (const point_t*)b;

    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

point_t move_point(image_t image, point_t point, int x, int y) {
    point.x = max(min(point.x + x, image.width - 1), 0);
    point.y = max(min(point.y + y, image.height - 1), 0);
    return point;
}

point_t rotate_point(point_t point, point_t centre, double angle) {
    double rad = angle * M_PI / 180;
    double sinr = sin(rad);
    double cosr = cos(rad);
    double dx = point.x - centre.x;
    double dy = point.y - centre.y;

    point.x = dx * cosr - dy * sinr + centre.x;
    point.y = dx * sinr + dy * cosr + centre.y;
    return point;
}

void draw_triangle(image_t image, point_t points[3], pixel_t color) {
    int y0 = points[0].y;
    int y2 = points[2].y;

    // Clip
    if (y2 < 0 || y0 >= image.height) return;
    if (y0 < 0) y0 = 0;
    if (y2 >= image.height) y2 = image.height - 1;

    for (int y = y0; y <= y2; y++) {
        int start, end;
        if (y < (int)points[1].y) {
            start = interpolate(y, points[0], points[1]);
            end = interpolate(y, points[0], points[2]);
        } else {
            start = interpolate(y, points[1], points[2]);
            end = interpolate(y, points[0], points[2]);
        }

        // Swap
        if (start > end) {
            int tmp = start;
            start = end;
            end = tmp;
        }

        // Clip
        if (end < 0 || start >= image.width) continue;
        if (start < 0) start = 0;
        if (end >= image.width) end = image.width - 1;

        for (int x = start; x <= end; x++) {
            draw_pixel(image, x, y, color);
        }
    }
}

void draw_polygon(image_t image, point_t* points, size_t pointc, pixel_t color) {
    if (pointc < 3) return;
    point_t triangle[3];
    for (size_t i = 1; i < pointc - 1; i++) {
        triangle[0] = points[0];
        triangle[1] = points[i];
        triangle[2] = points[i + 1];
        qsort(triangle, 3, sizeof(point_t), point_cmp);
        draw_triangle(image, triangle, color);
    }
}

void draw_rectangle(image_t image, point_t centre, int width, int height, pixel_t color,
                    double theta) {
    point_t points[4];
    int xi[] = {-1, 1, 1, -1};
    int yi[] = {-1, -1, 1, 1};
    for (int i = 0; i < 4; ++i) {
        points[i] = rotate_point(
            move_point(image, centre, xi[i] * (width / 2), yi[i] * (height / 2)), centre, theta);
    }
    draw_polygon(image, points, 4, color);
}

int in_ellipse(int x, int y, point_t centre, ld inv_a2, ld inv_b2, ld cosr, ld sinr) {
    ld dx = ((ld)x - (ld)centre.x);
    ld dy = ((ld)y - (ld)centre.y);
    ld xp = dx * cosr - dy * sinr;
    ld yp = dx * sinr + dy * cosr;
    return ((xp * xp) * inv_a2 + (yp * yp) * inv_b2) <= 1.0;
}

void draw_ellipse(image_t image, point_t centre, int width, int height, pixel_t color,
                  double angle) {
    ld rad = angle * M_PI / 180;
    ld sinr = sin(rad);
    ld cosr = cos(rad);
    ld inv_a2 = 2.0 / (ld)width;
    ld inv_b2 = 2.0 / (ld)height;

    inv_a2 *= inv_a2;
    inv_b2 *= inv_b2;

    for (int x = 0; x < image.width; ++x) {
        for (int y = 0; y < image.height; ++y) {
            if (in_ellipse(x, y, centre, inv_a2, inv_b2, sinr, cosr)) {
                draw_pixel(image, x, y, color);
            }
        }
    }
}
