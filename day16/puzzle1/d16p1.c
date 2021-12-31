#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../common/utils.h"

void process_packets(int *binary, int *index, int number, int *v_sum) {

    *v_sum += binary_to_int_range(binary, *index, 3);
    *index += 3;

    int id = binary_to_int_range(binary, *index, 3);
    *index += 3;

    if (id == 4) {
        // String literal
        while (binary[*index] == 1) {
            *index += 5;
        }
        *index += 5;
        // take care of 0s?
    } else {
        // Operator
        if (binary[(*index)++] == 0) {
            // 15 bits -> number of bits of all subpackets within
            int bit_count = binary_to_int_range(binary, *index, 15);
            *index += 15;

            int sub_end = *index + bit_count;
            while (*index < sub_end) {
                process_packets(binary, index, bit_count, v_sum);
            }
        } else {
            // 11 bits -> number of subpackets within
            int subpackets = binary_to_int_range(binary, *index, 11);
            *index += 11;

            for (int i = 0; i < subpackets; i++) {
                process_packets(binary, index, number, v_sum);
            }
        }
    }
}

int main() {

    FILE *input;

    if ((input = fopen("../input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char c;
    int hex_as_int[2048];
    int hex_length = 0;
    while(fscanf(input, "%c", &c) == 1) {
        hex_as_int[hex_length++] = ctox(c);
    }

    const int hex_map[16][4] = {
        { 0, 0, 0, 0 }, // 0
        { 0, 0, 0, 1 }, // 1
        { 0, 0, 1, 0 }, // 2
        { 0, 0, 1, 1 }, // 3
        { 0, 1, 0, 0 }, // 4
        { 0, 1, 0, 1 }, // 5
        { 0, 1, 1, 0 }, // 6
        { 0, 1, 1, 1 }, // 7
        { 1, 0, 0, 0 }, // 8
        { 1, 0, 0, 1 }, // 9
        { 1, 0, 1, 0 }, // A
        { 1, 0, 1, 1 }, // B
        { 1, 1, 0, 0 }, // C
        { 1, 1, 0, 1 }, // D
        { 1, 1, 1, 0 }, // E
        { 1, 1, 1, 1 }  // F
    };
    int binary_string[hex_length * 4];
    for (int hex = 0; hex < hex_length; hex++) {
        for (int i = 0; i < 4; i++) {
            binary_string[(hex * 4) + i] = hex_map[hex_as_int[hex]][i];
        }
    }

    int version_sum = 0;
    int index = 0;
    process_packets(binary_string, &index, hex_length * 4, &version_sum);

    printf("The sum of version numbers is %d\n", version_sum);

    fclose(input);
    return EXIT_SUCCESS;
}