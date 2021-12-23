#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 676
#define STEPS 10

typedef struct Pair {
    char pair[3];
    int count;
    int cnt_q;
} Pair;

typedef struct Instruction {
    char pair[3];
    char element;
} Instruction;

void print_pairs(Pair pairs[SIZE], int size) {

    for (int i = 0; i < size; i++) {
        printf("%s: %d\n", pairs[i].pair, pairs[i].count);
    }
}

int find_pair(char pair[3], Pair pairs[SIZE], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(pair, pairs[i].pair) == 0) {
            return i;
        }
    }

    return -1;
}

int main() {

    FILE *input;

    if ((input = fopen("../test_input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }


    char first, second;
    int pair_count = 0;
    int index = 0;
    Pair pairs[SIZE];
    char pair[3];
    fscanf(input, "%c", &first);
    do {

        fscanf(input, "%c", &second);

        if (second == '\n') {
            break;
        } else {
            pair[0] = first;
            pair[1] = second;
            pair[2] = '\0';
            index = find_pair(pair, pairs, pair_count);
            if (index == -1) {

                strcpy(pairs[pair_count].pair, pair);
                pairs[pair_count].count = 1;
                pairs[pair_count].cnt_q = 0;
                pair_count++;
            } else {
                pairs[index].count++;
            }
        }

    } while (first != '\n' && second != '\n');

    Instruction iset[SIZE];
    int icount = 0;
    while (fscanf(input, "%s -> %c\n", pair, &first) == 2) {
        memcpy(iset[icount].pair, pair, sizeof pair);
        iset[icount].element = first;
        icount++;
    }

    for (int step = 0; step < STEPS; step++) {
        for (int i = 0; i < icount; i++) {
            /*
             * For each instruction...
             * If the pair exists (in array with count > 0), dec cnt_q by 1
             * For the two resulting pairs create them if they don't exist
             * Set count to 0 and cnt_q to 1
             * If they do exist inc cnt_q by 1
             */
        }
        /*
         * For each pair...
         * Add cnt_q to count
         * Set cnt_q to 0
         */
    }

    fclose(input);
    return EXIT_SUCCESS;
}