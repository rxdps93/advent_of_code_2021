#include <stdio.h>
#include <stdlib.h>
#include "../../common/utils.h"

typedef struct {
	int x;
	int y;
	int z;
} Window;

void window_init(Window *win) {
	win->x = -1;
	win->y = -1;
	win->z = -1;
}

int window_sum(Window *win) {
	return win->x + win->y + win->z;
}

int main() {
	
	FILE *input;
	
	if ((input = fopen("input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	IntArray measurements;
	init_IntArray(&measurements, 1024);

	int value = 0;
	while(fscanf(input, "%d", &value) == 1) {
		insert_IntArray(&measurements, value);
	}
	fclose(input);

	Window currentWindow;
	Window previousWindow;
	window_init(&currentWindow);
	window_init(&previousWindow);

	int count = 0;
	for (int i = 0; i < measurements.size - 3; i++) {
		currentWindow.x = measurements.array[i];
		currentWindow.y = measurements.array[i + 1];
		currentWindow.z = measurements.array[i + 2];

		printf("%d, %d, %d\t%d\n",
			measurements.array[i],
			measurements.array[i + 1],
			measurements.array[i + 2],
			window_sum(&currentWindow));

		break;

		if ((window_sum(&currentWindow) > window_sum(&previousWindow))) {
			count++;
		}
		printf("prev:\t%d\t|\tcurr:\t%d\n", window_sum(&previousWindow), window_sum(&currentWindow));
		previousWindow = currentWindow;
	}

	printf("The depth increased %d times.\n", count);
	free_IntArray(&measurements);
	return EXIT_SUCCESS;
}