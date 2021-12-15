#include <stdio.h>
#include <stdlib.h>
#include "../../common/utils.h"

#define SIZE	10
#define STEPS	100

typedef struct Octopus Octopus;

struct Octopus {
	int power_level;

	Octopus *adjacent[8];
};

void print_octopuses(Octopus octopuses[SIZE][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			printf("%d", octopuses[row][col].power_level);
		}
		printf("\n");
	}
}

void octopus_flash(Octopus octopuses[SIZE][SIZE]) {

}

void increase_power_level(Octopus octopuses[SIZE][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			octopuses[row][col].power_level++;
		}
	}
}

void map_octopuses(Octopus octopuses[SIZE][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			// Up Left
			if (row != 0 && col != 0) {
				octopuses[row][col].adjacent[0] = octopuses[row - 1][col - 1];
			} else {
				octopuses[row][col].adjacent[0] = NULL;
			}

			// Up Middle

			// Up Right

			// Left

			// Right

			// Down Left

			// Down Middle

			// Down Right
		}
	}
}

int main() {

	FILE *input;
	
	if ((input = fopen("../test_input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	char octo;
	int row_ptr = 0;
	int col_ptr = 0;
	Octopus octopuses[SIZE][SIZE];
	Octopus octopus;
	while (fscanf(input, "%c", &octo) == 1) {

		if (octo != '\n') {
			octopus.power_level = ctoi(octo);
			octopuses[row_ptr][col_ptr++] = octopus;
		} else {
			row_ptr++;
			col_ptr = 0;
		}
	}

	map_octopuses(octopuses);
	for (int i = 0; i < STEPS; i++) {
		increase_power_level(octopuses);
		octopus_flash(octopuses);
	}

	fclose(input);
	return EXIT_SUCCESS;
}