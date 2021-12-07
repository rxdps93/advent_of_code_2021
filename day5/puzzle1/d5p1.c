#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 10

void mark_coords(int x1, int y1, int x2, int y2, int coords[ARR_SIZE][ARR_SIZE]) {

	if (x1 == x2) {
		printf("Processing vertical pairs: (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
		for (int i = y1; i <= y2; i++) {
			coords[x1][i]++;
		}
	} else if (y1 == y2) {
		printf("Processing horizontal pairs: (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
		for (int i = x1; i <= x2; i++) {
			coords[i][y1]++;
		}
	} else {
		printf("Skipping non-linear pairs: (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
	}
}

int main() {

	FILE *input;
	
	if ((input = fopen("../test_input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	// Read coordinates line by line, determine which coordinate pairs are impacted, and increment their count
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int coords[ARR_SIZE][ARR_SIZE] = {0};
	while (fscanf(input, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) == 4) {
		mark_coords(x1, y1, x2, y2, coords);
	}

	for (int row = 0; row < ARR_SIZE; row++) {
		for (int col = 0; col < ARR_SIZE; col++) {
			printf("%d ", coords[col][row]);
		}
		printf("\n");
	}

	fclose(input);
	return EXIT_SUCCESS;
}