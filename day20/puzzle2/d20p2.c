#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../common/utils.h"

#define TEST 0
#define STEPS 50
#define ALGORITHM_SIZE 512
#define INPUT_SIZE (TEST ? 5 : 100)

void print_img(const int *img, const int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%d", img[row * size + col]);
        }
        printf("\n");
    }
}

int map_char(char c) {
    return c == '.' ? 0 : c == '#' ? 1 : -1;
}

void img_copy(int *to, int *from, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            to[row * size + col] = from[row * size + col];
        }
    }
}

int trim_img(int **input_img, int old_size) {

    int new_size = old_size - 2;
    int new_img[new_size][new_size];

    for (int row = 0; row < new_size; row++) {
        for (int col = 0; col < new_size; col++) {
            new_img[row][col] = (*input_img)[(row + 1) * old_size + (col + 1)];
        }
    }

    *input_img = (int *)realloc(*input_img, new_size * new_size * sizeof(int));
    memcpy(*input_img, new_img, sizeof new_img);

    return new_size;
}

int pad_img(int **input_img, int old_size, int filler) {

    int new_size = old_size + 4;
    int new_img[new_size][new_size];

    for (int row = 0; row < new_size; row++) {
        for (int col = 0; col < new_size; col++) {
            new_img[row][col] = filler;
        }
    }

    for (int row = 0; row < old_size; row++) {
        for (int col = 0; col < old_size; col++) {
            new_img[row + 2][col + 2] = (*input_img)[row * old_size + col];
        }
    }

    *input_img = (int *)realloc(*input_img, new_size * new_size * sizeof(int));
    memcpy(*input_img, new_img, new_size * new_size * sizeof(int));

    return new_size;
}

int count_pixels(const int *img, const int size) {

    int count = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (img[row * size + col]) {
                count++;
            }
        }
    }
    return count;
}

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Get algorithm
    int algorithm[ALGORITHM_SIZE];
    for (int i = 0; i < ALGORITHM_SIZE; i++) {
        algorithm[i] = map_char(fgetc(input));
    }
    fseek(input, 2, SEEK_CUR);

    // Get input map <accessing array is row * [# COLS] + col == [row][col]>
    int *input_img = (int *)malloc(INPUT_SIZE * INPUT_SIZE * sizeof(int));
    for (int row = 0; row < INPUT_SIZE; row++) {
        for (int col = 0; col < INPUT_SIZE; col++) {
            input_img[row * INPUT_SIZE + col] = map_char(fgetc(input));
        }
        fgetc(input);
    }
    fclose(input);

    // Iterate for X steps:
    // Pad array with some (at least 2?) layers of the appropriate value (0 or 1, changes each time)
    // Output = Input
    // Start writing changes to output
    // Input = Output
    // Trim output down to an appropriate size
    int filler = 0;
    int size = INPUT_SIZE;
    int *output_img = (int *)malloc(size * size * sizeof(int));
    for (int i = 0; i < STEPS; i++) {

        size = pad_img(&input_img, size, filler);

        if (!TEST) {
            filler = 1 - filler;
        }

        output_img = (int *)realloc(output_img, size * size * sizeof(int));
        memcpy(output_img, input_img, size * size * sizeof(int));

        int grid[9];
        for (int row = 0; row < size - 2; row++) {
            for (int col = 0; col < size - 2; col++) {
                // input_img[row * size + col] = top left of 3x3 grid
                grid[0] = input_img[(row + 0) * size + (col + 0)];
                grid[1] = input_img[(row + 0) * size + (col + 1)];
                grid[2] = input_img[(row + 0) * size + (col + 2)];

                grid[3] = input_img[(row + 1) * size + (col + 0)];
                grid[4] = input_img[(row + 1) * size + (col + 1)];
                grid[5] = input_img[(row + 1) * size + (col + 2)];

                grid[6] = input_img[(row + 2) * size + (col + 0)];
                grid[7] = input_img[(row + 2) * size + (col + 1)];
                grid[8] = input_img[(row + 2) * size + (col + 2)];

                output_img[(row + 1) * size + (col + 1)] = algorithm[binary_to_int_range(grid, 0, 9)];
            }
        }

        memcpy(input_img, output_img, size * size * sizeof(int));
        size = trim_img(&input_img, size);
    }

    // Count lit pixels
    int lit_pixels = count_pixels(input_img, size);
    printf("There are %d pixels lit\n", lit_pixels);

    free(output_img);
    free(input_img);
    return EXIT_SUCCESS;
}