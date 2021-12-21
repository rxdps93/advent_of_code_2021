#include <stdio.h>
#include <stdlib.h>

#define STEPS 10

typedef struct Pair {
    char pair[3];
    int count;
} Pair;

typedef struct Instruction {
    char pair[3];
    char element;
} Instruction;

int main() {

    FILE *input;

    if ((input = fopen("../test_input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }



    fclose(input);
    return EXIT_SUCCESS;
}