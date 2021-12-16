#include <stdio.h>
#include <stdlib.h>
#include "../../common/utils.h"

#define SIZE	10
#define ADJ		8

typedef struct Octopus Octopus;

struct Octopus {
	int power_level;

	Octopus *adjacent[ADJ];
};

void print_octopuses(Octopus octopuses[SIZE][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			printf("%2d ", octopuses[row][col].power_level);
		}
		printf("\n");
	}
}

void octopus_flash(Octopus *octopus, int *flashes) {

	// octopus->power_level = 0;
	*flashes += 1;
	for (int i = 0; i < ADJ; i++) {
		if (octopus->adjacent[i]) {
			octopus->adjacent[i]->power_level++;

			if (octopus->adjacent[i]->power_level == 10) {
				octopus_flash(octopus->adjacent[i], flashes);
			}
		}
	}
}

int increase_power_level(Octopus octopuses[SIZE][SIZE], int *flashes) {

	int num_flashed = 0;
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {

			octopuses[row][col].power_level++;

			if (octopuses[row][col].power_level == 10) {
				octopus_flash(&octopuses[row][col], flashes);
			}
		}
	}

	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			if (octopuses[row][col].power_level > 9) {
				num_flashed++;
				octopuses[row][col].power_level = 0;
			}
		}
	}

	return num_flashed;
}

void map_octopuses(Octopus octopuses[SIZE][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			// Up Left
			if (row != 0 && col != 0) {
				octopuses[row][col].adjacent[0] = &octopuses[row - 1][col - 1];
			} else {
				octopuses[row][col].adjacent[0] = NULL;
			}

			// Up Middle
			if (row != 0) {
				octopuses[row][col].adjacent[1] = &octopuses[row - 1][col];
			} else {
				octopuses[row][col].adjacent[1] = NULL;
			}

			// Up Right
			if (row != 0 && col < (SIZE - 1)) {
				octopuses[row][col].adjacent[2] = &octopuses[row - 1][col + 1];
			} else {
				octopuses[row][col].adjacent[2] = NULL;
			}

			// Left
			if (col != 0) {
				octopuses[row][col].adjacent[3] = &octopuses[row][col - 1];
			} else {
				octopuses[row][col].adjacent[3] = NULL;
			}

			// Right
			if (col < (SIZE - 1)) {
				octopuses[row][col].adjacent[4] = &octopuses[row][col + 1];
			} else {
				octopuses[row][col].adjacent[4] = NULL;
			}

			// Down Left
			if (row < (SIZE - 1) && col != 0) {
				octopuses[row][col].adjacent[5] = &octopuses[row + 1][col - 1];
			} else {
				octopuses[row][col].adjacent[5] = NULL;
			}

			// Down Middle
			if (row < (SIZE - 1)) {
				octopuses[row][col].adjacent[6] = &octopuses[row + 1][col];
			} else {
				octopuses[row][col].adjacent[6] = NULL;
			}

			// Down Right
			if (row < (SIZE - 1) && col < (SIZE - 1)) {
				octopuses[row][col].adjacent[7] = &octopuses[row + 1][col + 1];
			} else {
				octopuses[row][col].adjacent[7] = NULL;
			}
		}
	}
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
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

	printf("Before any steps:\n");
	print_octopuses(octopuses);
	printf("\n");

	map_octopuses(octopuses);

	int flashes = 0;
	int num_flashed = 0;
	int flash_step = 0;
	do {
		printf("After step %d:\n", flash_step + 1);
		num_flashed = increase_power_level(octopuses, &flashes);
		print_octopuses(octopuses);
		printf("\n");
		flash_step++;
	} while (num_flashed != SIZE * SIZE);

	printf("All octopuses flashed together on step %d. The total number of flashes was %d.\n", flash_step, flashes);
	fclose(input);
	return EXIT_SUCCESS;
}