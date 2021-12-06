#include <stdio.h>
#include <stdlib.h>
#include "../../common/utils.h"

typedef struct {
    int row;
    int col;
} Pair;

typedef struct {
    int numbers[5][5];
    Pair marked[25];
    Pair winning_pos;
} Board;

int main() {

	FILE *input;
	
	if ((input = fopen("../test_input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}
    // Store input line
    int num;
    IntArray num_lot;
    init_IntArray(&num_lot, 10);
    while (fscanf(input, "%d,", &num) == 1) {
        printf("%d\n", num);
        insert_IntArray(&num_lot, num);
    }

    // Read in board to struct, determine score, check if higher than current highest
    free_IntArray(&num_lot);
    fclose(input);
    return EXIT_SUCCESS;
}