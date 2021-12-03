#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_LEN 12

int ctoi(char c) {
	return (int)c - '0';
}

int bintoi(int *binstr) {
	int dec = 0;
	int mul = 1;
	for (int i = BIN_LEN - 1; i >= 0; i--) {
		dec += binstr[i] * mul;
		mul *= 2;
	}
	return dec;
}

int main() {
	
	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	char binary[1024][BIN_LEN];
	int index = 0;
	while (fscanf(input, "%s", binary[index]) == 1) {
		index++;
	}
	fclose(input);

	index = 0;
	int ones_count[BIN_LEN] = { 0 };
	int value = 0;
	int bin_cnt = 0;
	while ((strcmp(binary[index], "\0") != 0) && strcmp(binary[index], "\n") != 0) {
		for (int i = 0; i < BIN_LEN; i++) {
			value = ctoi(binary[index][i]);
			if (value < 0) {
				break;
			}
			ones_count[i] += value;
		}
		bin_cnt++;
		index++;
	}

	int gamma_bin[BIN_LEN] = { 0 }; // most common
	int epsilon_bin[BIN_LEN] = { 0 }; // second most common
	for (int i = 0; i < BIN_LEN; i++) {
		if (ones_count[i] >= (bin_cnt / 2)) {
			gamma_bin[i] = 1;
			epsilon_bin[i] = 0;
		} else {
			epsilon_bin[i] = 1;
			gamma_bin[i] = 0;
		}
	}

	int gamma = bintoi(gamma_bin);
	int epsilon = bintoi(epsilon_bin);
	printf("Gamma Value:\t%d\nEpsilon Value:\t%d\nConsumption:\t%d\n", gamma, epsilon, gamma * epsilon);
	return EXIT_SUCCESS;
}