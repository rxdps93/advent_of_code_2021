#include <stdio.h>
#include <stdlib.h>

#define TEST 1
#define STEPS 2
#define ALGORITHM_SIZE 512
#define INPUT_SIZE (TEST ? 5 : 100)

int map_char(char c) {
    return c == '.' ? 0 : c == '#' ? 1 : -1;
}

char map_int(int i) {
    return i ? '#' : '.';
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

    for (int r = 0; r < INPUT_SIZE; r++) {
        for (int c = 0; c < INPUT_SIZE; c++) {
            printf("%d", input_img[r * INPUT_SIZE + c]);
        }
        printf("\n");
    }

    // Iterate for X steps:
    // Pad array with some (at least 2?) layers of the appropriate value (0 or 1, changes each time)
    // Output = Input
    // Start writing changes to output
    // Save output as new input

    return EXIT_SUCCESS;
}