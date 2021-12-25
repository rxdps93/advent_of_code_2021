#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 676
#define ELMNTS 26
#define STEPS 40

typedef struct Pair {
    char pair[3];
    long count;
    long cnt_q;
} Pair;

typedef struct Instruction {
    char pair[3];
    char element;
} Instruction;

void print_pairs(Pair pairs[SIZE], int size) {

    for (int i = 0; i < size; i++) {
        printf("%s: %ld\n", pairs[i].pair, pairs[i].count);
    }
}

void print_elements(long element_counts[ELMNTS], char unique_elements[ELMNTS], int unique_count) {

    for (int i = 0; i < unique_count; i++) {
        printf("%c: %ld\n", unique_elements[i], element_counts[i]);
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

void split_pair(Pair pair, Pair pairs[SIZE], int *pair_size, Instruction i) {

    char new_pair[3];
    int index = 0;
    
    // pair 1: pairs[index].pair[0] + iset[i].element
    new_pair[0] = pair.pair[0];
    new_pair[1] = i.element;
    new_pair[2] = '\0';
    index = find_pair(new_pair, pairs, *pair_size);

    if (index != -1) {
        pairs[index].cnt_q += pair.count;
    } else {
        strcpy(pairs[*pair_size].pair, new_pair);
        pairs[*pair_size].count = 0;
        pairs[*pair_size].cnt_q = pair.count;
        (*pair_size)++;
    }

    // pair 2: iset[i].element + pairs[index].pair[1]
    new_pair[0] = i.element;
    new_pair[1] = pair.pair[1];
    index = find_pair(new_pair, pairs, *pair_size);

    if (index != -1) {
        pairs[index].cnt_q += pair.count;
    } else {
        strcpy(pairs[*pair_size].pair, new_pair);
        pairs[*pair_size].count = 0;
        pairs[*pair_size].cnt_q = pair.count;
        (*pair_size)++;
    }
}

void process_queue(Pair pairs[SIZE], int pair_size) {

    for (int i = 0; i < pair_size; i++) {
        pairs[i].count += pairs[i].cnt_q;
        pairs[i].cnt_q = 0;

        if (pairs[i].count < 0) {
            // printf("theoretically this shouldn't happen...\n");
            pairs[i].count = 0;
        }
    }
}

void count_element(long increment, char element, long element_counts[ELMNTS], char unique_elements[ELMNTS], int unique_count) {
    for (int i = 0; i < unique_count; i++) {
        if (unique_elements[i] == element) {
            element_counts[i] += increment;
        }
    }
}

void log_unique_element(char element, char unique_elements[ELMNTS], int *unique_count) {

    for (int i = 0; i < *unique_count; i++) {
        if (unique_elements[i] == element) {
            return;
        }
    }

    unique_elements[(*unique_count)++] = element;
}

int main() {

    FILE *input;

    if ((input = fopen("../input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }


    char first, second;
    int pair_count = 0;
    int index = 0;
    Pair pairs[SIZE];
    char pair[3];
    char unique_elements[ELMNTS];
    long element_counts[ELMNTS] = { 0 };
    int unique_count = 0;
    fscanf(input, "%c", &first);
    do {

        fscanf(input, "%c", &second);

        if (second == '\n') {
            count_element(1, first, element_counts, unique_elements, unique_count);
            break;
        } else {
            log_unique_element(first, unique_elements, &unique_count);
            log_unique_element(second, unique_elements, &unique_count);

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

            count_element(1, first, element_counts, unique_elements, unique_count);
            first = second;
        }

    } while (first != '\n' && second != '\n');

    Instruction iset[SIZE];
    int icount = 0;
    while (fscanf(input, "%s -> %c\n", pair, &first) == 2) {
        memcpy(iset[icount].pair, pair, sizeof pair);
        iset[icount].element = first;
        icount++;

        log_unique_element(first, unique_elements, &unique_count);
    }

    for (int step = 0; step < STEPS; step++) {
        for (int i = 0; i < icount; i++) {

            index = find_pair(iset[i].pair, pairs, pair_count);
            if (index != -1) {
                if (pairs[index].count > 0) {
                    count_element((long)pairs[index].count, iset[i].element, element_counts, unique_elements, unique_count);
                    pairs[index].cnt_q -= pairs[index].count;
                    split_pair(pairs[index], pairs, &pair_count, iset[i]);
                }
            }
        }

        process_queue(pairs, pair_count);
    }

    long count = 0;
    long max_elmnt = LONG_MIN;
    long min_elmnt = LONG_MAX;
    int max_index = -1;
    int min_index = -1;
    for (int unique = 0; unique < unique_count; unique++) {

        count = element_counts[unique];

        if (count > max_elmnt) {
            max_elmnt = count;
            max_index = unique;
        }

        if (count < min_elmnt) {
            min_elmnt = count;
            min_index = unique;
        }
    }

    print_elements(element_counts, unique_elements, unique_count);

    printf("Ran for %d steps.\n", STEPS);
    printf("Most Common (%c: %ld) minus Least Common (%c: %ld): %ld\n",
        unique_elements[max_index], max_elmnt,
        unique_elements[min_index], min_elmnt,
        max_elmnt - min_elmnt);

    fclose(input);
    return EXIT_SUCCESS;
}