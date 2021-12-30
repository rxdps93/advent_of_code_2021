#include <stdio.h>
#include <stdlib.h>

#define HEX_0 { 0, 0, 0, 0}

int main() {

    FILE *input;

    if ((input = fopen("../test_files/test_input_16.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char c;
    char hex_input[2048];
    int hex_length = 0;
    while(fscanf(input, "%c", &c) == 1) {
        hex_input[hex_length++] = c;
    }
    printf("Length: %d -> %s\n", hex_length, hex_input);

    int binary_string[hex_length * 4];
    for (int hex = 0; hex < hex_length; hex++) {
        for (int i = 0; i < 4; i++) {
            // binary_string[(hex * 4) + i] = 0;
        }
    }

    fclose(input);
    return EXIT_SUCCESS;
}