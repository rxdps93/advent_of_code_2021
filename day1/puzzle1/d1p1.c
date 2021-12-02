#include <stdio.h>
#include <stdlib.h>

int main() {
	
	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	int value = 0;
	int previous = -1;
	int count = 0;
	while(fscanf(input, "%d", &value) == 1) {
		if (value > previous && previous != -1) {
			count++;
		}
		previous = value;
	}
	fclose(input);
	printf("The depth increased %d times.\n", count);

	return EXIT_SUCCESS;
}
