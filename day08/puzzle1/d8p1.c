#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGIT_ONE_LEN	2
#define DIGIT_FOUR_LEN	4
#define DIGIT_SEVEN_LEN	3
#define DIGIT_EIGHT_LEN	7

/*
 * DIGIT	SEGMENTS
 *	0			6
 *	1			2	UNIQUE
 *	2			5
 *	3			5
 *	4			4	UNIQUE
 *	5			5
 *	6			6
 *	7			3	UNIQUE
 *	8			7	UNIQUE
 *	9			6
 */
int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	int count = 0;
	char digit_in[10][8];
	char digit_out[4][8];
	while (fscanf(input, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s", 
		digit_in[0], digit_in[1], digit_in[2], digit_in[3], digit_in[4], digit_in[5], digit_in[6], digit_in[7], digit_in[8], digit_in[9],
		digit_out[0], digit_out[1], digit_out[2], digit_out[3]) == 14) {
		
		for (int digit = 0; digit < 4; digit++) {
			switch (strlen(digit_out[digit])) {
				case DIGIT_ONE_LEN:
				case DIGIT_FOUR_LEN:
				case DIGIT_SEVEN_LEN:
				case DIGIT_EIGHT_LEN:
					count++;
				default:
					break;
			}
		}
	}
	
	printf("Instances of '1', '4', '7', '8': %d\n", count);
	fclose(input);
	return EXIT_SUCCESS;
}