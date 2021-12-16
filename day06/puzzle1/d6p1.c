#include <stdio.h>
#include <stdlib.h>
#include "../../common/utils.h"

#define NUM_DAYS 80
#define TXT_SINGLE "day"
#define TXT_PLURAL "days"

void cycle_school(IntArray *school) {

	int limit = (int)school->used;
	for (int i = 0; i < limit; i++) {
		if (school->array[i] == 0) {
			insert_IntArray(school, 8);
			school->array[i] = 6;
		} else {
			school->array[i]--;
		}
	}
}

int main() {
	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	IntArray fish_school;
	init_IntArray(&fish_school, 1);
	int fish;
	while (fscanf(input, "%d,", &fish) == 1) {
		insert_IntArray(&fish_school, fish);
	}

	for (int day = 0; day < NUM_DAYS; day++) {
		cycle_school(&fish_school);
	}

	printf("After %d days there are %zu lanternfish in the school.\n", NUM_DAYS, fish_school.used);
	free_IntArray(&fish_school);
	fclose(input);
	return EXIT_SUCCESS;
}
