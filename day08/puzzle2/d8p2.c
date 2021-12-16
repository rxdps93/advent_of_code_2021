#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEN_ZERO_SIX_NINE	6
#define LEN_ONE				2
#define LEN_TWO_THREE_FIVE	5
#define LEN_FOUR			4
#define LEN_SEVEN			3
#define LEN_EIGHT			7

int comp_char(const void *a, const void *b) {
	return *(char*)a - *(char*)b;
}

// Find 1, 4, 7, 8
void find_uniques(char digit[10][8], char digit_sorted[10][8]) {
	for (int i = 0; i < 10; i++) {
		qsort(digit[i], (size_t)strlen(digit[i]), sizeof(char), comp_char);
		switch(strlen(digit[i])) {
			case LEN_ONE: // Found digit 1
				memcpy(digit_sorted[1], digit[i], sizeof digit[i]);
				break;
			case LEN_FOUR: // Found digit 4
				memcpy(digit_sorted[4], digit[i], sizeof digit[i]);
				break;
			case LEN_SEVEN: // Found digit 7
				memcpy(digit_sorted[7], digit[i], sizeof digit[i]);
				break;
			case LEN_EIGHT: // Found digit 8
				memcpy(digit_sorted[8], digit[i], sizeof digit[i]);
				break;
		}
	}
}

// Find 3, 6
void find_three_and_six(char digit[10][8], char digit_sorted[10][8], char digit_len_five[2][8], char digit_len_six[2][8]) {

	int fives = 0;
	int sixes = 0;
	for (int i = 0; i < 10; i++) {
		if (strlen(digit[i]) == LEN_TWO_THREE_FIVE) {
			if (strchr(digit[i], digit_sorted[1][0]) && strchr(digit[i], digit_sorted[1][1])) { // Found digit 3
				memcpy(digit_sorted[3], digit[i], sizeof digit[i]);
			} else {
				memcpy(digit_len_five[fives], digit[i], sizeof digit[i]); // Candidates for digits 2 and 5...
				fives++;
			}
		} else if (strlen(digit[i]) == LEN_ZERO_SIX_NINE) {
			if ((strchr(digit[i], digit_sorted[1][0]) && !strchr(digit[i], digit_sorted[1][1])) ||
				(!strchr(digit[i], digit_sorted[1][0]) && strchr(digit[i], digit_sorted[1][1]))) { // Found digit 6

				memcpy(digit_sorted[6], digit[i], sizeof digit[i]);
			} else {
				memcpy(digit_len_six[sixes], digit[i], sizeof digit[i]); // Candidates for digits 0 and 9...
				sixes++;
			}
		}
	}
}

// Find 0, 2, 5, 9
void find_remaining_digits(char digit_sorted[10][8], char digit_len_five[2][8], char digit_len_six[2][8]) {

	int cnt_five, cnt_six, digit;
	for (int segment = 0; segment < 8; segment++) {

		cnt_five = 0;
		cnt_six = 0;
		for (int i = 0; i < 2; i++) {
			if (strchr(digit_len_five[i], digit_sorted[8][segment])) {
				cnt_five++;
			}
			if (strchr(digit_len_six[i], digit_sorted[8][segment])) {
				cnt_six++;
			}
		}

		if (cnt_five == 2 && cnt_six == 1) {
			digit = (strchr(digit_len_six[1], digit_sorted[8][segment]) == NULL);
			memcpy(digit_sorted[0], digit_len_six[digit], sizeof digit_len_six[digit]); // Found digit 0
			memcpy(digit_sorted[9], digit_len_six[1 - digit], sizeof digit_len_six[1 - digit]); // Found digit 9
		}
	}

	int is_missing;
	for (cnt_five = 0; cnt_five < 2; cnt_five++) {

		is_missing = 0;
		for (int segment = 0; segment < 5; segment++) {
			is_missing += (strchr(digit_sorted[9], digit_len_five[cnt_five][segment]) == NULL);
		}

		if (is_missing == 0) {
			memcpy(digit_sorted[5], digit_len_five[cnt_five], sizeof digit_len_five[cnt_five]);
			memcpy(digit_sorted[2], digit_len_five[1 - cnt_five], sizeof digit_len_five[1 - cnt_five]);
		}
	}
}

void identify_digits(char digit[10][8], char digit_sorted[10][8], char digit_len_five[2][8], char digit_len_six[2][8]) {
	find_uniques(digit, digit_sorted);
	find_three_and_six(digit, digit_sorted, digit_len_five, digit_len_six);
	find_remaining_digits(digit_sorted, digit_len_five, digit_len_six);
}

int get_digit(char sequence[8], char digit_list[10][8]) {
	for (int i = 0; i < 10; i++) {
		if (strcmp(sequence, digit_list[i]) == 0) {
			return i;
		}
	}
	return -1;
}

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
 * 
 * PROCESS:
 * 1. sort each string from a->g
 * 2. the 5 length digit that contains all of 1 must be 3
 * 3. the 6 length digit that is missing one part of 1 must be 6
 * FOR THE 4 REMAINING STRINGS
 * 1. the 6 length one that is missing a part of the other 3 must be 0
 * 2. the remaining 6 length must be 9
 * 3. the 5 length one that is fully contained within 9, minus 1 segment, must be 5
 * 4. the remaining 5 length must be 2
 */
int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	char digit_in[10][8];
	char digit_out[4][8];
	char digit_sorted[10][8];
	char digit_len_five[2][8];
	char digit_len_six[2][8];
	int final_number = 0;
	while (fscanf(input, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s", 
		digit_in[0], digit_in[1], digit_in[2], digit_in[3], digit_in[4], digit_in[5], digit_in[6], digit_in[7], digit_in[8], digit_in[9],
		digit_out[0], digit_out[1], digit_out[2], digit_out[3]) == 14) {

		identify_digits(digit_in, digit_sorted, digit_len_five, digit_len_six);

		for (int i = 0; i < 4; i++) {
			qsort(digit_out[i], (size_t)strlen(digit_out[i]), sizeof(char), comp_char);
			final_number += pow(10, (3 - i)) * get_digit(digit_out[i], digit_sorted);
		}
	}
	
	printf("Final Value: %d\n", final_number);

	fclose(input);
	return EXIT_SUCCESS;
}