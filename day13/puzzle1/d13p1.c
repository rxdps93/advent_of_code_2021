#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARR_SIZE 2048
#define HORIZONTAL 'y'
#define VERTICAL 'x'

typedef struct Fold {
    char axis;
    int value;
} Fold;

void print_paper(int paper[ARR_SIZE][ARR_SIZE], int rows, int cols) {
    printf("   ");
    for (int i = 0; i < cols; i++) {
        printf("%d", i);
    }
    printf("\n");

    for (int row = 0; row < rows; row++) {
        printf("%2d ", row);
        for (int col = 0; col < cols; col++) {
            if (paper[row][col] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf(" %2d\n", row);
    }
    printf("   ");
    for (int i = 0; i < cols; i++) {
        printf("%d", i);
    }
    printf("\n");
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

void fold_paper(Fold fold, int paper[ARR_SIZE][ARR_SIZE], int *rows, int *cols) {

    if (fold.axis == HORIZONTAL) { // fold on y=...

        int index = 0;
        for (int row = *rows - 1; row > fold.value; row--) {
            for (int col = 0; col < *cols; col++) {
                index = row - (2 * (row - fold.value));
                paper[index][col] = paper[index][col] || paper[row][col];
                // printf("Overlapping (%d, %d) with (%d, %d)...", row, col, index, col);
                // printf("%d || %d -> %d\n", paper[index][col], paper[row][col], paper[index][col] || paper[row][col]);
            }
        }

        *rows = fold.value;

    } else if (fold.axis == VERTICAL) { //fold on x=...
        
        int index = 0;
        for (int col = *cols - 1; col > fold.value; col--) {
            for (int row = 0; row < *rows; row++) {
                index = col - (2 * (col - fold.value));
                paper[row][index] = paper[row][index] || paper[row][col];
                // printf("Overlapping (%d, %d) with (%d, %d)...", row, col, row, index);
                // printf("%d || %d -> %d\n", paper[row][index], paper[row][col], paper[row][index] || paper[row][col]);
            }
        }

        *cols = fold.value;

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
        
        // printf("Adding dot at (%d, %d)\n", dot_x, dot_y);
        paper[dot_y][dot_x] = 1;

        if (dot_x > x_max) {
            x_max = dot_x;
        }

        if (dot_y > y_max) {
            y_max = dot_y;
        }
    }
    printf("%d, %d\n", x_max, y_max);
    x_max++;
    y_max++;

    Fold folds[ARR_SIZE];
    int fold_count = 0;
    while (fscanf(input, "fold along %c=%d ", &folds[fold_count].axis, &folds[fold_count].value) > 0) {
        fold_count++;
    }
 
    fold_paper(folds[0], paper, &y_max, &x_max);
    // fold_paper(folds[1], paper, &y_max, &x_max);
    // print_paper(paper, y_max, x_max);
    printf("After the folds there are %d dots.\n", count_dots(paper, y_max, x_max));

    fclose(input);
    return EXIT_SUCCESS;
}