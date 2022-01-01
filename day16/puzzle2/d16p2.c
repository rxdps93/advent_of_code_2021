#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../common/utils.h"

long long process_packets(int *binary, int *index, int number, int *v_sum) {

    *v_sum += binary_to_int_range(binary, *index, 3);
    *index += 3;

    int id = binary_to_int_range(binary, *index, 3);
    *index += 3;

    long long packet_value = 0;
    if (id == 4) {
        // String literal
        int bin_lit[1024];
        int lit_index = 0;
        while (binary[*index] == 1) {
            for (int i = 0; i < 4; i++) {
                bin_lit[lit_index++] = binary[(*index + 1) + i];
            }
            *index += 5;
        }

        for (int i = 0; i < 4; i++) {
            bin_lit[lit_index++] = binary[(*index + 1) + i];
        }
        *index += 5;

        packet_value += binary_to_llong_range(bin_lit, 0, lit_index);
    } else {
        // Operator
        int sub_index = 0;
        long long subpacket_values[1024];
        if (binary[(*index)++] == 0) {
            // 15 bits -> number of bits of all subpackets within
            int bit_count = binary_to_int_range(binary, *index, 15);
            *index += 15;
            int stop = *index + bit_count;
            while (*index < stop) {
                subpacket_values[sub_index++] = process_packets(binary, index, bit_count, v_sum);
            }
        } else {
            // 11 bits -> number of subpackets within
            int subpackets = binary_to_int_range(binary, *index, 11);
            *index += 11;

            for (int i = 0; i < subpackets; i++) {
                subpacket_values[sub_index++] = process_packets(binary, index, number, v_sum);
            }
        }

        // Process subpackets here (stored above)
        switch (id) {
            case 0: // sum
                for (int i = 0; i < sub_index; i++) {
                    packet_value += subpacket_values[i];
                }
                break;
            case 1: {
                // product
                long long product = 1;
                for (int i = 0; i < sub_index; i++) {
                    product *= subpacket_values[i];
                }
                packet_value = product;
                break;
            }
            case 2: {
                // min
                long long min = LLONG_MAX;
                for (int i = 0; i < sub_index; i++) {
                    if (subpacket_values[i] < min) {
                        min = subpacket_values[i];
                    }
                }
                packet_value = min;
                break;
            }
            case 3: {
                // max
                long long max = LLONG_MIN;
                for (int i = 0; i < sub_index; i++) {
                    if (subpacket_values[i] > max) {
                        max = subpacket_values[i];
                    }
                }
                packet_value = max;
                break;
            }
            case 5: // gt (always 2 subs)
                packet_value = (subpacket_values[0] > subpacket_values[1]);
                break;
            case 6: // lt (always 2 subs)
                packet_value = (subpacket_values[0] < subpacket_values[1]);
                break;
            case 7: // eq (always 2 subs)
                packet_value = (subpacket_values[0] == subpacket_values[1]);
                break;
        }
    }

    return packet_value;
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
    long long value = process_packets(binary_string, &index, hex_length * 4, &version_sum);

    printf("The value is %lld\n", value);

    fclose(input);
    return EXIT_SUCCESS;
}