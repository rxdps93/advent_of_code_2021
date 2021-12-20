#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARR_SIZE 1024

typedef struct Fold {
    char axis;
    int value;
} Fold;

void print_paper(int paper[ARR_SIZE][ARR_SIZE]) {
    for (int row = 0; row < 14; row++) {
        for (int col = 0; col < 10; col++) {
            if (paper[row][col] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main() {

    FILE *input;
	
	if ((input = fopen("../test_input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

    int paper[ARR_SIZE][ARR_SIZE] = { 0 };
    int x_max = INT_MIN;
    int y_max = INT_MIN;
    int dot_x, dot_y;
    while (fscanf(input, "%d,%d", &dot_x, &dot_y) == 2) {
        
        printf("Adding dot at (%d, %d)\n", dot_x, dot_y);
        paper[dot_y][dot_x] = 1;

        if (dot_x > x_max) {
            x_max = dot_x;
        }

        if (dot_y > y_max) {
            y_max = dot_y;
        }

        print_paper(paper);
    }

    Fold folds[ARR_SIZE];
    Fold fold;
    int fold_count = 0;
    int val;
    char c;
    while (fscanf(input, "fold along %c=%d", &c, &val) == 2) {
        fold.axis = c;
        fold.value = val;
        folds[fold_count++] = fold;
    }

    fclose(input);
    return EXIT_SUCCESS;
}