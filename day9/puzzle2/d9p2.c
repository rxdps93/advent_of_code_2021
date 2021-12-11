#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../common/utils.h"

#define LINE_SIZE 10

typedef struct HeightRating HeightRating;

struct HeightRating {
	int height;
	bool is_low_point;

	bool is_mapped;
	HeightRating *up;
	HeightRating *down;
	HeightRating *left;
	HeightRating *right;
};

typedef struct {
	HeightRating items[1024];
} Queue;

int find_low_points(HeightRating ratings[LINE_SIZE][LINE_SIZE], int row_ptr) {
	int low_point_count = 0;
	for (int row = 0; row < row_ptr; row++) {

		for (int col = 0; col < LINE_SIZE; col++) {

			// Check up
			if (row != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row - 1][col].height;
				ratings[row][col].up = &ratings[row - 1][col];
			}

			// Check down
			if (ratings[row][col].is_low_point && row < row_ptr - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row + 1][col].height;
				ratings[row][col].down = &ratings[row + 1][col];
			}

			// Check left
			if (ratings[row][col].is_low_point && col != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col - 1].height;
				ratings[row][col].left = &ratings[row][col - 1];
			}

			// Check right
			if (ratings[row][col].is_low_point && col < LINE_SIZE - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col + 1].height;
				ratings[row][col].right = &ratings[row][col + 1];
			}

			if (ratings[row][col].is_low_point) {
				low_point_count++;
			}
		}
	}

	return low_point_count;
}

int map_basin(HeightRating node) {

	int size = 1;
	return size;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../test_input.txt", "r")) == NULL) {
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
			rating.is_mapped = false;
			rating.up = NULL;
			rating.down = NULL;
			rating.left = NULL;
			rating.right = NULL;
			ratings[row_ptr][col_ptr++] = rating;
		} else {
			row_ptr++;
			col_ptr = 0;
		}
	}

	int basin_sizes[find_low_points(ratings, row_ptr)];
	int basin = 0;
	for (int row = 0; row < row_ptr; row++) {
		for (int col = 0; col < LINE_SIZE; col++) {
			if (ratings[row][col].is_low_point) {
				basin_sizes[basin++] = map_basin(ratings[row][col]);
			}
		}
	}

	fclose(input);
	return EXIT_SUCCESS;
}