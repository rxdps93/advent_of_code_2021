#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../common/utils.h"

#define BIN_LEN 12
#define BIN_ROW 1024
#define BIN_COL (BIN_LEN + 1)

int bintoi(int *binstr) {
	int dec = 0;
	int mul = 1;
	for (int i = BIN_LEN - 1; i >= 0; i--) {
		dec += binstr[i] * mul;
		mul *= 2;
	}
	return dec;
}

void ctoi_arr(char *binstr, int *arr) {
	for (int i = 0; i < BIN_LEN; i++) {
		arr[i] = ctoi(binstr[i]);
	}
}

int generate_subset(char master[BIN_ROW][BIN_COL], char subset[BIN_ROW][BIN_COL], int pos, int val, int size) {
	// Populate subset with each value of master that has value val at position pos
	int index_master = 0;
	int index_subset = 0;
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (ctoi(master[index_master][pos]) == val) {
			memcpy(subset[index_subset], master[index_master], sizeof(master[index_master]));
			count++;
			index_subset++;
		}
		index_master++;
	}

	return count;
}

int count_ones(int ones_count[BIN_LEN], char master[BIN_ROW][BIN_COL], int size) {
	int value = 0;
	int bin_cnt = 0;
	memset(ones_count, 0, BIN_LEN * sizeof(int));
	for (int index = 0; index < size; index++) {
		for (int i = 0; i < BIN_LEN; i++) {
			value = ctoi(master[index][i]);
			if (value < 0) {
				break;
			}
			ones_count[i] += value;
		}
		bin_cnt++;
	}

	return bin_cnt;
}

int main() {
	
	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	char binary[BIN_ROW][BIN_COL];
	int index = 0;
	while (fscanf(input, "%s", binary[index]) == 1) {
		index++;
	}
	fclose(input);

	int ones_count[BIN_LEN] = { 0 };
	int bin_cnt = count_ones(ones_count, binary, index);
	char master[BIN_ROW][BIN_COL];
	char subset[BIN_ROW][BIN_COL];
	int sub_ones[BIN_LEN];
	int array[BIN_LEN];
	int life_support = 1;
	int rating = 0;
	int value, sub_cnt;
	for (int i = 0; i < 2; i++) {
		memcpy(master, binary, sizeof(binary));
		memcpy(sub_ones, ones_count, BIN_LEN * sizeof(int));
		sub_cnt = bin_cnt;
		for (int j = 0; j < BIN_LEN; j++) {
			if (!i) {
				value = (sub_ones[j] >= (sub_cnt - sub_ones[j]));
			} else {
				value = (sub_ones[j] < (sub_cnt - sub_ones[j]));
			}

			sub_cnt = generate_subset(master, subset, j, value, sub_cnt);
			memcpy(master, subset, sizeof(subset));
			count_ones(sub_ones, master, sub_cnt);
			if (sub_cnt == 1) {
				break;
			}
		}
		ctoi_arr(subset[0], array);
		rating = bintoi(array);
		life_support *= rating;
	}

	printf("Life Support:\t%d\n", life_support);
	return EXIT_SUCCESS;
}
