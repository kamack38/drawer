#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const pixel_t WRONG_PIXEL = (pixel_t){0, 1, 1};

pixel_t** bitmap_alloc(int width, int height) {
    pixel_t** image = malloc(height * sizeof(pixel_t*));
    if (image == NULL) return NULL;

    pixel_t* data = calloc(width * height, sizeof(pixel_t));
    if (data == NULL) {
        free(image);
        return NULL;
    }
    // Set color of all pixels to white
    memset(data, MAX_COLOR, width * height * sizeof(pixel_t));

    for (int x = 0; x < height; x++) {
        image[x] = data + x * width;
    }

    return image;
}

int write_image(image_t image, FILE* output, int binary) {
    fprintf(output, "P%d\n", binary ? 6 : 3);
    fprintf(output, "%d %d %d\n", image.width, image.height, MAX_COLOR);
    if (binary) {
        fwrite(image.bitmap[0], sizeof(pixel_t), image.height * image.width, output);
    } else {
        for (int y = 0; y < image.height; y++) {
            for (int x = 0; x < image.width; x++) {
                fprintf(output, "%d %d %d ", image.bitmap[y][x].r, image.bitmap[y][x].g,
                        image.bitmap[y][x].b);
            }
            fprintf(output, "\n");
        }
    }
    free(image.bitmap[0]);
    free(image.bitmap);
    return 0;
}

pixel_t parse_color(char* str, size_t len) {
    if (strncmp(str, "black", len) == 0) {
        return (pixel_t){0, 0, 0};
    } else if (strncmp(str, "white", len) == 0) {
        return (pixel_t){1, 1, 1};
    } else if (strncmp(str, "red", len) == 0) {
        return (pixel_t){1, 0, 0};
    } else if (strncmp(str, "green", len) == 0) {
        return (pixel_t){0, 1, 0};
    } else if (strncmp(str, "blue", len) == 0) {
        return (pixel_t){0, 0, 1};
    } else if (strncmp(str, "yellow", len) == 0) {
        return (pixel_t){1, 1, 0};
    }
    return WRONG_PIXEL;
}

int pixel_eq(pixel_t a, pixel_t b) {
    if (a.r == b.r && a.g == b.g && a.b == b.b) {
        return 1;
    }
    return 0;
}
