#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../common/utils.h"

#define LINE_SIZE 100

typedef struct HeightRating HeightRating;

struct HeightRating {
	int height;
	bool is_low_point;
};

int find_low_points(HeightRating ratings[LINE_SIZE][LINE_SIZE], int row_ptr) {
	int total_risk_factor = 0;
	for (int row = 0; row < row_ptr; row++) {

		for (int col = 0; col < LINE_SIZE; col++) {

			// Check up
			if (row != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row - 1][col].height;
			}

			// Check down
			if (ratings[row][col].is_low_point && row < row_ptr - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row + 1][col].height;
			}

			// Check left
			if (ratings[row][col].is_low_point && col != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col - 1].height;
			}

			// Check right
			if (ratings[row][col].is_low_point && col < LINE_SIZE - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col + 1].height;
			}

			if (ratings[row][col].is_low_point) {
				total_risk_factor += ratings[row][col].height + 1;
			}
		}
	}

	return total_risk_factor;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	HeightRating ratings[LINE_SIZE][LINE_SIZE];
	HeightRating rating;
	char height;
	int row_ptr = 0;
	int col_ptr = 0;
	while (fscanf(input, "%c", &height) == 1) {

		if (height != '\n') {
			rating.height = ctoi(height);
			rating.is_low_point = true;
			ratings[row_ptr][col_ptr++] = rating;
		} else {
			row_ptr++;
			col_ptr = 0;
		}
	}

	printf("Total Risk Factor: %d\n", find_low_points(ratings, row_ptr));
	fclose(input);
	return EXIT_SUCCESS;
}