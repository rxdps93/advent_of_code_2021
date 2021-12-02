#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORWARD	"forward"
#define DOWN	"down"
#define UP		"up"

int main() {

	FILE *input;
	int depth = 0;
	int position = 0;
	
	if ((input = fopen("input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	char command[80];
	int value = 0;
	int unk = 0;
	while (fscanf(input, "%s %d", command, &value) == 2) {
		if (strcmp(FORWARD, command) == 0) {
			position += value;
		} else if (strcmp(DOWN, command) == 0) {
			depth += value;
		} else if (strcmp(UP, command) == 0) {
			depth -= value;
		} else {
			printf("UNKNOWN COMMAND:\t%s %d - SKIPPING\n", command, value);
			unk++;
		}
	}
	fclose(input);

	printf("Final depth:\t%d\nFinal position:\t%d\nFINAL VALUE:\t%d\n",
		depth, position, depth * position);
	if (unk > 0) {
		printf("Unknown:\t%d\n", unk);
	}
	return EXIT_SUCCESS;
}