#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fgetline(FILE* file, char** buf, size_t* size) {
    if (buf == NULL || size == NULL) return -1;
    if (*buf == NULL) {
        *buf = malloc(64);
        if (*buf == NULL) {
            fprintf(stderr, "Failed to allocate memory for the buffer!\n");
            return -1;
        }
        *size = 64;
    }

    size_t pos = 0;
    (*buf)[*size - 1] = EOF;

    while (1) {
        if (fgets(*buf + pos, *size - pos, file) == NULL) {
            if (ferror(file)) {
                perror("Error reading from file");
                return -2;
            }
            // Reached EOF
            return 1;
        }

        // Read full line
        if ((*buf)[*size - 1] == EOF || ((*buf)[*size - 1] == '\0' && (*buf)[*size - 2] == '\n')) {
            break;
        }

        // Buffer is full
        pos = *size - 1;
        size_t new_cap = *size * 2;
        char* new_buf = realloc(*buf, new_cap * sizeof(char));
        if (new_buf == NULL) {
            fprintf(stderr, "Failed to reallocate the buffer!\n");
            return -1;
        }
        *buf = new_buf;
        *size = new_cap;

        (*buf)[*size - 1] = EOF;
    }
    return 0;
}

char* skip_whitespace(char* buffer) {
    if (buffer == NULL) return NULL;
    return buffer + strspn(buffer, " \t\n\r");
}

int has_trailing_words(char* buffer) {
    if (buffer == NULL) return 0;
    if (*(buffer + strspn(buffer, " \t\n\r")) == '\0') {
        return 0;
    }
    return 1;
}
