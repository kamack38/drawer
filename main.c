#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "image.h"
#include "line.h"
#include "vector.h"

void eprint(int line, const char* format, ...) {
    va_list args;
    va_start(args, format);

    fprintf(stderr, "Error: Line %d: ", line);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

int main(const int argc, const char* argv[]) {
    int binary = 0;
    const char* file_name = NULL;
    FILE* output = stdout;
    image_t image = {0};

    if (argc > 3 || (argc >= 2 && (strcmp(argv[1], "-h") == 0))) {
        fprintf(stderr, "Usage: %s [-b] [output.ppm]\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0) {
            binary = 1;
            continue;
        }
        file_name = argv[i];
    }
    if (file_name != NULL) {
        output = fopen(file_name, binary ? "wb" : "w");
        if (output == NULL) {
            perror("Unable to open output file");
        }
    }

    if (scanf("%u %u ", &image.width, &image.height) != 2) {
        fprintf(stderr, "Error: Unable to read image dimensions\n");
        return 1;
    }
    image.bitmap = bitmap_alloc(image.width, image.height);
    if (image.bitmap == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for the image\n");
        return 1;
    }

    char* buffer = NULL;
    size_t size = 0;
    point_t* points = vector(point_t);
    for (int line = 1; fgetline(stdin, &buffer, &size) == 0; ++line) {
        char* buf = skip_whitespace(buffer);
        size_t len = strcspn(buf, " \t\n\r");
        int object = parse_object(buf, len);
        if (object == -1) {
            eprint(line, "Unknown shape '%.*s'", (int)len, buf);
            continue;
        }

        buf += len;
        buf = skip_whitespace(buf);
        len = strcspn(buf, " \t\n\r");
        pixel_t pixel = parse_color(buf, len);
        if (pixel_eq(pixel, WRONG_PIXEL)) {
            eprint(line, "Unknown color '%.*s'", (int)len, buf);
            continue;
        }

        buf += len;

        if (object == POLYGON) {
            point_t point;
            int offset;
            while (sscanf(buf, "%d %d%n", &point.x, &point.y, &offset) == 2) {
                push(points, &point);
                buf += offset;
            }
            draw_polygon(image, points, length(points), pixel);
            // Clear vector
            vector_header(points)->size = 0;
        } else {  // Object is a rectangle or an ellipse
            point_t centre;
            int width, height;
            double theta = 0.0;
            int offset;
            if (sscanf(buf, "%d %d%n", &centre.x, &centre.y, &offset) != 2) {
                eprint(line, "Wrong centre point coordinates!");
                continue;
            }
            buf += offset;
            if (sscanf(buf, "%d %d%n", &width, &height, &offset) != 2) {
                eprint(line, "Wrong width or height!");
                continue;
            }
            buf += offset;
            sscanf(buf, "%lf%n", &theta, &offset);
            buf += offset;

            if (object == RECTANGLE) {
                draw_rectangle(image, centre, width, height, pixel, theta);
            } else if (object == ELLIPSE) {
                draw_ellipse(image, centre, width, height, pixel, theta);
            }
        }
        if (has_trailing_words(buf)) {
            fprintf(stderr, "Warning: Line %d: Trailing parameters ignored!\n", line);
        }
    }
    write_image(image, output, binary);
    free(buffer);
    vector_free(points);
    return 0;
}
