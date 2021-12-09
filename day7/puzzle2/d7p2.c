#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define ARR_SIZE 1024

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	int *crabs;
	crabs = malloc(ARR_SIZE * sizeof *crabs);
	int min_pos = INT_MAX;
	int max_pos = INT_MIN;
	int count = 0;
	int num;
	while(fscanf(input, "%d,", &num) == 1) {
		crabs[count] = num;
		count++;
		if (num > max_pos) {
			max_pos = num;
		} else if (num < min_pos) {
			min_pos = num;
		}
	}

	int fuel_cost = 0;
	int best_fuel = INT_MAX;
	int best_pos = 0;
	int diff = 0;
	for (int pos = min_pos; pos <= max_pos; pos++) {
		fuel_cost = 0;
		for (int crab = 0; crab < count; crab++) {
			diff = abs(crabs[crab] - pos);
			fuel_cost += (diff * (diff + 1)) / 2;
		}
		if (fuel_cost < best_fuel) {
			best_fuel = fuel_cost;
			best_pos = pos;
		}
	}

	printf("The best position is %d with a fuel cost of %d.\n", best_pos, best_fuel);

	fclose(input);
	return EXIT_SUCCESS;
}