#include <stdio.h>
#include <stdlib.h>
#include "../utils/d23_pqueue.h"

#define TEST 1

void parse_input(FILE *input) {

    burrow_t state;

    char c;
    char str[50];
    int count = 0;
    while (fscanf(input, "%[^A,B,C,D]s", str) == 1) {
        c = fgetc(input);

        amphipod_t amphi;
        switch (c) {
            case 'A':
                amphi.type = AMBER;
                break;
            case 'B':
                amphi.type = BRONZE;
                break;
            case 'C':
                amphi.type = COPPER;
                break;
            case 'D':
                amphi.type = DESERT;
                break;
        }

        if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {

        }

        printf("%c", c);
        count++;

        if (count == 4 || count == 8) {
            printf("\n");

            if (count == 8) {
                break;
            }
        }
    }

    // return state;
}

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }
    parse_input(input);
    // burrow_t state = parse_input(input);

    fclose(input);
    return EXIT_SUCCESS;
}