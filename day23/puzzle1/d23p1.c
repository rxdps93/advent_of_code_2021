#include <stdio.h>
#include <stdlib.h>
#include "../utils/day23.h"

#define TEST 1

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    burrow_queue_t pq;
    queue_init(&pq, 10);

    burrow_t b_arr[10] = { { 7 }, { 12 }, { -3 }, { 42 }, { 0 }, { 9 }, { -4 }, { 15293 }, { 2 }, { 1 } };
    for (int i = 0; i < 10; i++) {
        queue_add(&pq, b_arr[i]);
    }

    burrow_t b = { 14 };
    queue_add(&pq, b);

    queue_remove(&pq, &b);
    printf("I removed %d\n", b.energy);
    printf("New capacity is %lu\n", pq.capacity);

    print_queue(&pq);

    queue_free(&pq);
    fclose(input);
    return EXIT_SUCCESS;
}