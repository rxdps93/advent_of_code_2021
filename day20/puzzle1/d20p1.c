#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST 1
#define STEPS 1
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

int pad_input(int *input_img, int old_size, int filler) {

    int new_size = old_size + 4;
    int new_img[new_size][new_size];
    
    for (int i = 0; i < new_size; i++) {
        new_img[0][i] = filler;
        new_img[1][i] = filler;
        new_img[new_size - 2][i] = filler;
        new_img[new_size - 1][i] = filler;

        new_img[i][0] = filler;
        new_img[i][1] = filler;
        new_img[i][new_size - 2] = filler;
        new_img[i][new_size - 1] = filler;
    }

    for (int row = 0; row < old_size; row++) {
        for (int col = 0; col < old_size; col++) {
            new_img[row + 2][col + 2] = input_img[row * old_size + col];
        }
    }

    input_img = realloc(input_img, new_size * new_size * sizeof(int));
    memcpy(input_img, new_img, sizeof new_img);

    return new_size;
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
    int *input_img = malloc(INPUT_SIZE * INPUT_SIZE * sizeof(int));
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
    // Trim output down to an appropriate size
    // Save output as new input
    int size = INPUT_SIZE;
    int filler = 0;
    for (int i = 0; i < STEPS; i++) {
        size = pad_input(input_img, size, filler);
        filler = 1 - filler;
        print_img(input_img, size);
    }

    free(input_img);
    return EXIT_SUCCESS;
}