#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../common/utils.h"

#define NUM_DAYS 256
#define FISH_CYCLE 7
#define NEW_FISH_CYCLE 9

long calculate_school(IntArray *school) {

	long initial_fish = school->used;
	long fish_spawn[1024] = { 0 };
	long day, fish, new_fish, spawned_fish, spawns;
	// How many fish each initial fish will spawn spread out across the days
	for (fish = 0; fish < initial_fish; fish++) {

		spawned_fish = floor((NUM_DAYS - school->array[fish] - 1) / FISH_CYCLE);

		for (new_fish = 0; new_fish <= spawned_fish; new_fish++) {

			long spawn_day = school->array[fish] + (new_fish * FISH_CYCLE) + 1;
			fish_spawn[spawn_day]++;
		}
	}

	// How many fish the spawned fish will spawn spread out across the days
	for (day = 0; day < NUM_DAYS; day++) {

		new_fish = fish_spawn[day];

		if (fish_spawn[day] > 0) {

			spawned_fish = floor((NUM_DAYS - day - NEW_FISH_CYCLE) / FISH_CYCLE);
			for (fish = 0; fish <= spawned_fish; fish++) {

				spawns = day + NEW_FISH_CYCLE + (fish * FISH_CYCLE);
				fish_spawn[spawns] += new_fish;
			}
		}
	}

	// Go through each day and sum up the spawned fish
	spawned_fish = 0;
	for (day = 0; day < NUM_DAYS + 1; day++) {
		spawned_fish += fish_spawn[day];
	}

	return initial_fish + spawned_fish;
}

long cycle_school(IntArray *school) {

	for (long day = 0; day < NUM_DAYS; day++) {
		long limit = (long)school->used;
		for (long i = 0; i < limit; i++) {
			if (school->array[i] == 0) {
				mul_insert_IntArray(school, NEW_FISH_CYCLE - 1, 2);
				school->array[i] = FISH_CYCLE - 1;
			} else {
				school->array[i]--;
			}
		}
	}

	return (long)school->used;
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


	printf("Total Fish via Math: %ld\n", calculate_school(&fish_school));
	// Takes way too long for 256 but it works
	printf("Total Fish via Iter: %ld\n", cycle_school(&fish_school));

	free_IntArray(&fish_school);
	fclose(input);
	return EXIT_SUCCESS;
}
