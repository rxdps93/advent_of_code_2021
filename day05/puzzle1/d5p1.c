#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 1024

int count_coords(int coords[ARR_SIZE][ARR_SIZE], int prnt) {
	int count = 0;
	for (int row = 0; row < ARR_SIZE; row++) {
		for (int col = 0; col < ARR_SIZE; col++) {
			if (prnt) {
				printf("%d ", coords[col][row]);
			}
			if (coords[col][row] > 1) {
				count++;
			}
		}
		if (prnt) {
			printf("\n");
		}
	}
	return count;
}

void mark_coords(int x1, int y1, int x2, int y2, int coords[ARR_SIZE][ARR_SIZE]) {

	if (x1 == x2) {
		printf("Processing vertical pairs: (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);

		if (y1 < y2) {
			for (int i = y1; i <= y2; i++) {
				coords[x1][i]++;
			}
		} else {
			for (int i = y1; i >= y2; i--) {
				coords[x1][i]++;
			}
		}
	} else if (y1 == y2) {
		printf("Processing horizontal pairs: (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);

		if (x1 < x2) {
			for (int i = x1; i <= x2; i++) {
				coords[i][y1]++;
			}
		} else {
			for (int i = x1; i >= x2; i--) {
				coords[i][y1]++;
			}
		}
	} else {
		printf("Skipping diagonal pairs: (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
	}
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
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

	printf("FINAL COORDS:\n");
	printf("Count: %d\n", count_coords(coords, 0));

	fclose(input);
	return EXIT_SUCCESS;
}