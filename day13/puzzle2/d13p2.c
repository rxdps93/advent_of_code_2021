#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARR_SIZE 2048
#define HORIZONTAL 'y'
#define VERTICAL 'x'

void print_paper(int paper[ARR_SIZE][ARR_SIZE], int rows, int cols) {

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (paper[row][col] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int count_dots(int paper[ARR_SIZE][ARR_SIZE], int rows, int cols) {

    int dots = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (paper[row][col] == 1) {
                dots++;
            }
        }
    }

    return dots;
}

void fold_paper(char axis, int value, int paper[ARR_SIZE][ARR_SIZE], int *rows, int *cols) {

    if (axis == HORIZONTAL) { // fold on y=...

        int index = 0;
        for (int row = *rows - 1; row > value; row--) {
            for (int col = 0; col < *cols; col++) {
                index = row - (2 * (row - value));
                paper[index][col] = paper[index][col] || paper[row][col];
            }
        }

        *rows = value;

    } else if (axis == VERTICAL) { //fold on x=...
        
        int index = 0;
        for (int col = *cols - 1; col > value; col--) {
            for (int row = 0; row < *rows; row++) {
                index = col - (2 * (col - value));
                paper[row][index] = paper[row][index] || paper[row][col];
            }
        }

        *cols = value;

    } else {
        printf("oops\n");
    }
}

int main() {

    FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

    static int paper[ARR_SIZE][ARR_SIZE] = { 0 };
    int x_max = INT_MIN;
    int y_max = INT_MIN;
    int dot_x, dot_y;
    while (fscanf(input, "%d,%d", &dot_x, &dot_y) == 2) {
        
        paper[dot_y][dot_x] = 1;

        if (dot_x > x_max) {
            x_max = dot_x;
        }

        if (dot_y > y_max) {
            y_max = dot_y;
        }
    }
    x_max++;
    y_max++;

    int value;
    char axis;
    while (fscanf(input, "fold along %c=%d ", &axis, &value) > 0) {
        fold_paper(axis, value, paper, &y_max, &x_max);
    }

    print_paper(paper, y_max, x_max);
    printf("After the folds there are %d dots.\n", count_dots(paper, y_max, x_max));

    fclose(input);
    return EXIT_SUCCESS;
}